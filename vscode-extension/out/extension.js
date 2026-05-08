"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const vscode = __importStar(require("vscode"));
const cp = __importStar(require("child_process"));
const path = __importStar(require("path"));
const fs = __importStar(require("fs"));
let statusBarBtn;
let outputChannel;
function activate(context) {
    outputChannel = vscode.window.createOutputChannel('VEX Sim');
    statusBarBtn = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
    statusBarBtn.text = '$(play) Run Sim';
    statusBarBtn.tooltip = 'Build & launch the VEX V5 simulator with this project\'s code';
    statusBarBtn.command = 'vexsim.run';
    statusBarBtn.show();
    const cmd = vscode.commands.registerCommand('vexsim.run', runSim);
    context.subscriptions.push(cmd, statusBarBtn, outputChannel);
}
// Wrap a path in double-quotes for safe shell injection
function q(p) {
    return `"${p}"`;
}
async function runSim() {
    const cfg = vscode.workspace.getConfiguration('vexsim');
    const simDir = cfg.get('simDirectory', '').trim();
    const preset = cfg.get('cmakePreset', 'msys2-ucrt64-lemlib').trim();
    if (!simDir) {
        const open = 'Open Settings';
        const choice = await vscode.window.showErrorMessage('VEX Sim: Set vexsim.simDirectory to the path of your Vex Sim project.', open);
        if (choice === open) {
            vscode.commands.executeCommand('workbench.action.openSettings', 'vexsim.simDirectory');
        }
        return;
    }
    if (!fs.existsSync(path.join(simDir, 'CMakeLists.txt'))) {
        vscode.window.showErrorMessage(`VEX Sim: No CMakeLists.txt found in "${simDir}"`);
        return;
    }
    const prosRoot = vscode.workspace.workspaceFolders?.[0]?.uri.fsPath;
    if (!prosRoot) {
        vscode.window.showErrorMessage('VEX Sim: No workspace folder open.');
        return;
    }
    const srcDir = path.join(prosRoot, 'src');
    const includeDir = path.join(prosRoot, 'include');
    // Build dir comes from the preset's binaryDir (${sourceDir}/build)
    const buildDir = path.join(simDir, 'build');
    const exePath = path.join(buildDir, 'sim', 'vex_sim.exe');
    outputChannel.clear();
    outputChannel.show(true);
    outputChannel.appendLine(`PROS project:  ${prosRoot}`);
    outputChannel.appendLine(`Sim directory: ${simDir}`);
    outputChannel.appendLine(`CMake preset:  ${preset}`);
    outputChannel.appendLine('');
    // Kill any running vex_sim.exe so the linker can overwrite it
    await killRunningSimAsync();
    // Step 1: cmake configure using preset (handles generator + compilers)
    outputChannel.appendLine('── Configuring ──────────────────────────────');
    const configureArgs = [
        '--preset', preset,
        `-DROBOT_SRC_DIR=${q(srcDir)}`,
        `-DROBOT_INCLUDE_DIR=${q(includeDir)}`,
    ];
    const configured = await runCommand('cmake', configureArgs, simDir);
    if (!configured) {
        return;
    }
    // Step 2: cmake build
    outputChannel.appendLine('── Building ─────────────────────────────────');
    const built = await runCommand('cmake', ['--build', q(buildDir), '--target', 'vex_sim'], simDir);
    if (!built) {
        return;
    }
    // Step 3: launch — spawn directly so SDL2 window opens regardless of shell syntax
    outputChannel.appendLine('── Launching ────────────────────────────────');
    const r = cfg;
    const launchArgs = [
        '--wheel-radius', String(r.get('robot.wheelRadiusIn', 1.625)),
        '--track-width', String(r.get('robot.trackWidthIn', 12.0)),
        '--max-rpm', String(r.get('robot.maxRpm', 600)),
        '--mass', String(r.get('robot.massKg', 10.0)),
        '--drive-motors', String(r.get('robot.driveMotorCount', 6)),
        '--gear-friction', String(r.get('robot.gearFriction', 0.0)),
        '--left-ports', (r.get('robot.leftPorts', [4, 5, 6])).join(','),
        '--right-ports', (r.get('robot.rightPorts', [1, 2, 3])).join(','),
        '--tracking-left', String(r.get('robot.trackingLeftPort', 0)),
        '--tracking-right', String(r.get('robot.trackingRightPort', 0)),
        '--tracking-mid', String(r.get('robot.trackingMidPort', 0)),
        '--tracking-radius', String(r.get('robot.trackingWheelRadiusIn', 1.0)),
        '--robot-width', String(r.get('robot.widthIn', 8.0)),
        '--robot-height', String(r.get('robot.heightIn', 8.0)),
        '--start-x', String(r.get('robot.startX', 0.0)),
        '--start-y', String(r.get('robot.startY', 0.0)),
        '--start-heading', String(r.get('robot.startHeadingDeg', 0.0)),
    ];
    outputChannel.appendLine(exePath + ' ' + launchArgs.join(' '));
    const simCwd = path.join(buildDir, 'sim');
    const child = cp.spawn(exePath, launchArgs, { cwd: simCwd, detached: true, stdio: 'ignore' });
    child.unref(); // don't keep the extension host alive waiting for it
}
function killRunningSimAsync() {
    return new Promise(resolve => {
        // taskkill /F /IM vex_sim.exe — silently kills any running instance.
        // Exit code 128 means "no matching process", which is fine.
        const proc = cp.spawn('taskkill', ['/F', '/IM', 'vex_sim.exe'], { shell: true });
        proc.on('close', () => resolve());
    });
}
function msys2Env() {
    const ucrt64 = 'C:\\msys64\\ucrt64\\bin';
    const current = process.env.PATH ?? '';
    const already = current.toLowerCase().includes('ucrt64');
    return {
        ...process.env,
        PATH: already ? current : `${ucrt64};${current}`,
    };
}
function runCommand(cmd, args, cwd) {
    return new Promise(resolve => {
        const proc = cp.spawn(cmd, args, { cwd, shell: true, env: msys2Env() });
        proc.stdout.on('data', (d) => outputChannel.append(d.toString()));
        proc.stderr.on('data', (d) => outputChannel.append(d.toString()));
        proc.on('close', code => {
            if (code !== 0) {
                outputChannel.appendLine(`\nError: command exited with code ${code}`);
                vscode.window.showErrorMessage(`VEX Sim: ${cmd} failed (exit ${code}). See Output > VEX Sim for details.`);
                resolve(false);
            }
            else {
                resolve(true);
            }
        });
    });
}
function deactivate() { }
