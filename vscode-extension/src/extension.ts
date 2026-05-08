import * as vscode from 'vscode';
import * as cp from 'child_process';
import * as path from 'path';
import * as fs from 'fs';

let statusBarBtn: vscode.StatusBarItem;
let outputChannel: vscode.OutputChannel;

export function activate(context: vscode.ExtensionContext) {
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
function q(p: string): string {
    return `"${p}"`;
}

async function runSim() {
    const cfg = vscode.workspace.getConfiguration('vexsim');
    const simDir  = cfg.get<string>('simDirectory', '').trim();
    const preset  = cfg.get<string>('cmakePreset', 'msys2-ucrt64-lemlib').trim();

    if (!simDir) {
        const open = 'Open Settings';
        const choice = await vscode.window.showErrorMessage(
            'VEX Sim: Set vexsim.simDirectory to the path of your Vex Sim project.',
            open
        );
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

    const srcDir     = path.join(prosRoot, 'src');
    const includeDir = path.join(prosRoot, 'include');
    // Build dir comes from the preset's binaryDir (${sourceDir}/build)
    const buildDir   = path.join(simDir, 'build');
    const exePath    = path.join(buildDir, 'sim', 'vex_sim.exe');

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
    if (!configured) { return; }

    // Step 2: cmake build
    outputChannel.appendLine('── Building ─────────────────────────────────');
    const built = await runCommand(
        'cmake',
        ['--build', q(buildDir), '--target', 'vex_sim'],
        simDir
    );
    if (!built) { return; }

    // Step 3: launch — spawn directly so SDL2 window opens regardless of shell syntax
    outputChannel.appendLine('── Launching ────────────────────────────────');

    const r = cfg;
    const launchArgs = [
        '--wheel-radius',       String(r.get<number>('robot.wheelRadiusIn',       1.625)),
        '--track-width',        String(r.get<number>('robot.trackWidthIn',         12.0)),
        '--max-rpm',            String(r.get<number>('robot.maxRpm',               600)),
        '--mass',               String(r.get<number>('robot.massKg',               10.0)),
        '--drive-motors',       String(r.get<number>('robot.driveMotorCount',      6)),
        '--left-ports',         (r.get<number[]>('robot.leftPorts',  [4,5,6])).join(','),
        '--right-ports',        (r.get<number[]>('robot.rightPorts', [1,2,3])).join(','),
        '--tracking-left',      String(r.get<number>('robot.trackingLeftPort',     0)),
        '--tracking-right',     String(r.get<number>('robot.trackingRightPort',    0)),
        '--tracking-mid',       String(r.get<number>('robot.trackingMidPort',      0)),
        '--tracking-radius',    String(r.get<number>('robot.trackingWheelRadiusIn',1.0)),
        '--robot-width',        String(r.get<number>('robot.widthIn',              8.0)),
        '--robot-height',       String(r.get<number>('robot.heightIn',             8.0)),
    ];

    outputChannel.appendLine(exePath + ' ' + launchArgs.join(' '));

    const simCwd = path.join(buildDir, 'sim');
    const child = cp.spawn(exePath, launchArgs, { cwd: simCwd, detached: true, stdio: 'ignore' });
    child.unref();  // don't keep the extension host alive waiting for it
}

function killRunningSimAsync(): Promise<void> {
    return new Promise(resolve => {
        // taskkill /F /IM vex_sim.exe — silently kills any running instance.
        // Exit code 128 means "no matching process", which is fine.
        const proc = cp.spawn('taskkill', ['/F', '/IM', 'vex_sim.exe'], { shell: true });
        proc.on('close', () => resolve());
    });
}

function runCommand(cmd: string, args: string[], cwd: string): Promise<boolean> {
    return new Promise(resolve => {
        const proc = cp.spawn(cmd, args, { cwd, shell: true });

        proc.stdout.on('data', (d: Buffer) => outputChannel.append(d.toString()));
        proc.stderr.on('data', (d: Buffer) => outputChannel.append(d.toString()));

        proc.on('close', code => {
            if (code !== 0) {
                outputChannel.appendLine(`\nError: command exited with code ${code}`);
                vscode.window.showErrorMessage(
                    `VEX Sim: ${cmd} failed (exit ${code}). See Output > VEX Sim for details.`
                );
                resolve(false);
            } else {
                resolve(true);
            }
        });
    });
}

export function deactivate() {}
