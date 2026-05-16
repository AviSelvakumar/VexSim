import * as vscode from 'vscode';
import * as cp from 'child_process';
import * as path from 'path';
import * as fs from 'fs';
import * as os from 'os';

let statusBarBtn: vscode.StatusBarItem;
let outputChannel: vscode.OutputChannel;

const EXT_VERSION = '0.1.1';

export function activate(context: vscode.ExtensionContext) {
    outputChannel = vscode.window.createOutputChannel('VEX Sim');

    statusBarBtn = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
    statusBarBtn.text = '$(play) Run Sim';
    statusBarBtn.tooltip = 'Build & launch the VEX V5 simulator';
    statusBarBtn.command = 'vexsim.run';
    statusBarBtn.show();

    const runCmd   = vscode.commands.registerCommand('vexsim.run',   () => runSim(context));
    const setupCmd = vscode.commands.registerCommand('vexsim.setup', () => runSetup(context, true));
    context.subscriptions.push(runCmd, setupCmd, statusBarBtn, outputChannel);
}

// ── Paths ─────────────────────────────────────────────────────────────────────

function simDir(context: vscode.ExtensionContext): string {
    return context.globalStorageUri.fsPath;
}

function buildDir(context: vscode.ExtensionContext): string {
    return path.join(simDir(context), 'build');
}

// ── Setup ─────────────────────────────────────────────────────────────────────

async function ensureSetup(context: vscode.ExtensionContext): Promise<boolean> {
    const installed = context.globalState.get<string>('simVersion');
    if (installed === EXT_VERSION) return true;

    const msg = installed
        ? `VEX Sim updated to ${EXT_VERSION}. Re-run setup to apply changes.`
        : 'VEX Sim needs one-time setup before first use.';

    const choice = await vscode.window.showInformationMessage(msg, 'Set Up', 'Cancel');
    if (choice !== 'Set Up') return false;
    return runSetup(context, false);
}

async function runSetup(context: vscode.ExtensionContext, manual: boolean): Promise<boolean> {
    outputChannel.clear();
    outputChannel.show(true);
    outputChannel.appendLine('── VEX Sim Setup ────────────────────────────');

    // 1. Copy bundled sim source to writable global storage
    const dest   = simDir(context);
    const srcDir = path.join(context.extensionPath, 'sim-src');

    if (!fs.existsSync(srcDir)) {
        vscode.window.showErrorMessage('VEX Sim: sim-src not found in extension bundle. Reinstall the extension.');
        return false;
    }

    outputChannel.appendLine(`Copying sim source to ${dest}...`);
    await fs.promises.mkdir(dest, { recursive: true });
    await fs.promises.cp(srcDir, dest, { recursive: true, force: true });
    outputChannel.appendLine('Done.\n');

    // 2. Check and optionally install prerequisites
    const missing = getMissingPrereqs();
    if (missing.length > 0) {
        outputChannel.appendLine('Missing prerequisites: ' + missing.join(', '));
        const choice = await vscode.window.showWarningMessage(
            `VEX Sim: Missing prerequisites (${missing.join(', ')}). Install automatically?`,
            'Install', 'Skip'
        );
        if (choice === 'Install') {
            outputChannel.appendLine('\n── Installing prerequisites ─────────────────');
            const ok = await installPrerequisites();
            if (!ok) {
                vscode.window.showWarningMessage(
                    'VEX Sim: Prerequisite installation may have failed. Check Output > VEX Sim.'
                );
            }
        }
    } else {
        outputChannel.appendLine('All prerequisites found.');
    }

    await context.globalState.update('simVersion', EXT_VERSION);
    outputChannel.appendLine('\nSetup complete. Open a PROS project and click Run Sim.');
    if (manual) { vscode.window.showInformationMessage('VEX Sim setup complete!'); }
    return true;
}

function getMissingPrereqs(): string[] {
    const missing: string[] = [];
    if (!isOnPath('cmake'))                                                         missing.push('CMake');
    if (!fs.existsSync('C:\\msys64\\ucrt64\\bin\\gcc.exe'))                        missing.push('MSYS2 gcc');
    if (!fs.existsSync('C:\\msys64\\ucrt64\\bin\\mingw32-make.exe'))               missing.push('MSYS2 make');
    if (!fs.existsSync('C:\\msys64\\ucrt64\\bin\\lld.exe'))                        missing.push('MSYS2 lld');
    if (!fs.existsSync('C:\\msys64\\ucrt64\\lib\\cmake\\SDL2\\SDL2Config.cmake'))  missing.push('SDL2');
    return missing;
}

function isOnPath(exe: string): boolean {
    try { cp.execSync(`where ${exe}`, { stdio: 'ignore' }); return true; }
    catch { return false; }
}

function installPrerequisites(): Promise<boolean> {
    const lines = [
        '[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12',
        'if (Get-Command winget -ErrorAction SilentlyContinue) {',
        '    winget install Kitware.CMake MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements --silent',
        '} else {',
        '    $tmp = $env:TEMP',
        '    if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {',
        '        Write-Host "Finding latest CMake release..."',
        '        $json = curl.exe -s "https://api.github.com/repos/Kitware/CMake/releases/latest" | ConvertFrom-Json',
        '        $asset = $json.assets | Where-Object { $_.name -like "*windows-x86_64.msi" } | Select-Object -First 1',
        '        if (-not $asset) { Write-Error "Could not find CMake MSI in latest release"; exit 1 }',
        '        Write-Host "Downloading $($asset.name)..."',
        '        curl.exe -L --fail -o "$tmp\\cmake.msi" $asset.browser_download_url',
        '        if ($LASTEXITCODE -ne 0) { Write-Error "CMake download failed"; exit 1 }',
        '        Write-Host "Installing CMake..."',
        '        $r = Start-Process msiexec -ArgumentList "/i `"$tmp\\cmake.msi`" /quiet /norestart ADD_CMAKE_TO_PATH=System" -Wait -PassThru',
        '        if ($r.ExitCode -ne 0) { Write-Error "CMake install failed (exit $($r.ExitCode))"; exit 1 }',
        '    }',
        '    if (-not (Test-Path "C:\\msys64\\usr\\bin\\pacman.exe")) {',
        '        Write-Host "Downloading MSYS2..."',
        '        curl.exe -L --fail -o "$tmp\\msys2.exe" "https://github.com/msys2/msys2-installer/releases/latest/download/msys2-x86_64-latest.exe"',
        '        if ($LASTEXITCODE -ne 0) { Write-Error "MSYS2 download failed"; exit 1 }',
        '        Write-Host "Installing MSYS2..."',
        '        $r = Start-Process "$tmp\\msys2.exe" -ArgumentList "install --root C:\\msys64 --confirm-command --accept-messages" -Wait -PassThru',
        '        if ($r.ExitCode -ne 0) { Write-Error "MSYS2 install failed (exit $($r.ExitCode))"; exit 1 }',
        '    }',
        '}',
        'Write-Host "Installing MSYS2 packages..."',
        'C:\\msys64\\usr\\bin\\pacman.exe -S --noconfirm --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2',
        '$p = [System.Environment]::GetEnvironmentVariable("Path", "Machine")',
        'if ($p -notlike "*ucrt64\\bin*") { [System.Environment]::SetEnvironmentVariable("Path", $p + ";C:\\msys64\\ucrt64\\bin", "Machine") }',
    ];

    const tmpScript = path.join(os.tmpdir(), 'vexsim_prereqs.ps1');
    fs.writeFileSync(tmpScript, lines.join('\n'));

    return new Promise(resolve => {
        const proc = cp.spawn(
            'powershell.exe',
            ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', tmpScript],
            { shell: false }
        );
        proc.stdout?.on('data', (d: Buffer) => outputChannel.append(d.toString()));
        proc.stderr?.on('data', (d: Buffer) => outputChannel.append(d.toString()));
        proc.on('close', code => resolve(code === 0));
    });
}

function parseGearRatio(raw: unknown): number {
    const s = String(raw ?? '1:1').trim();
    if (s.includes(':')) {
        const [a, b] = s.split(':').map(Number);
        if (!isNaN(a) && !isNaN(b) && b > 0) return a / b;
    }
    const n = parseFloat(s);
    return isNaN(n) || n <= 0 ? 1.0 : n;
}

// ── Tool resolution ───────────────────────────────────────────────────────────

function findCmake(): string | undefined {
    const candidates = [
        'C:\\Program Files\\CMake\\bin\\cmake.exe',
        'C:\\Program Files (x86)\\CMake\\bin\\cmake.exe',
    ];
    for (const c of candidates) {
        if (fs.existsSync(c)) { return c; }
    }
    // Fall back to PATH (works if VS Code was launched after cmake was installed)
    try {
        const r = cp.execFileSync('where.exe', ['cmake'], { encoding: 'utf8' })
                    .trim().split('\n')[0]?.trim();
        if (r && fs.existsSync(r)) { return r; }
    } catch {}
    return undefined;
}

function buildEnv(): NodeJS.ProcessEnv {
    // Prepend MSYS2 UCRT64 bin so collect2 can find ld regardless of system PATH
    if (!fs.existsSync('C:\\msys64\\ucrt64\\bin\\gcc.exe')) { return process.env; }
    const current = process.env.PATH ?? '';
    const ucrt64  = 'C:\\msys64\\ucrt64\\bin';
    if (current.toLowerCase().includes('ucrt64')) { return process.env; }
    return { ...process.env, PATH: ucrt64 + ';' + current };
}

// ── Run Sim ───────────────────────────────────────────────────────────────────

async function runSim(context: vscode.ExtensionContext) {
    if (!await ensureSetup(context)) { return; }

    const cfg    = vscode.workspace.getConfiguration('vexsim');
    const preset = cfg.get<string>('cmakePreset', 'msys2-ucrt64-lemlib').trim();

    const prosRoot = vscode.workspace.workspaceFolders?.[0]?.uri.fsPath;
    if (!prosRoot) {
        vscode.window.showErrorMessage('VEX Sim: No workspace folder open.');
        return;
    }

    const srcDir     = path.join(prosRoot, 'src');
    const includeDir = path.join(prosRoot, 'include');
    const sd         = simDir(context);
    const bd         = buildDir(context);
    const exePath    = path.join(bd, 'sim', 'vex_sim.exe');

    outputChannel.clear();
    outputChannel.show(true);
    outputChannel.appendLine(`PROS project:  ${prosRoot}`);
    outputChannel.appendLine(`Sim directory: ${sd}`);
    outputChannel.appendLine(`CMake preset:  ${preset}`);
    outputChannel.appendLine('');

    const cmake = findCmake();
    if (!cmake) {
        vscode.window.showErrorMessage(
            'VEX Sim: cmake.exe not found. Run "VEX Sim: Setup" to install prerequisites.'
        );
        return;
    }
    outputChannel.appendLine(`CMake:         ${cmake}`);

    // Verify sim source was actually deployed by setup
    if (!fs.existsSync(path.join(sd, 'CMakeLists.txt'))) {
        vscode.window.showErrorMessage(
            'VEX Sim: sim source not found in storage. Run "VEX Sim: Setup" first.'
        );
        outputChannel.appendLine(`Error: CMakeLists.txt missing from ${sd}`);
        return;
    }

    await killRunningSimAsync();

    // Configure
    outputChannel.appendLine('── Configuring ──────────────────────────────');
    const configured = await runCommand(cmake, [
        '--preset', preset,
        `-DROBOT_SRC_DIR=${srcDir}`,
        `-DROBOT_INCLUDE_DIR=${includeDir}`,
    ], sd);
    if (!configured) { return; }

    // Build
    outputChannel.appendLine('── Building ─────────────────────────────────');
    const built = await runCommand(cmake, ['--build', bd, '--target', 'vex_sim'], sd);
    if (!built) { return; }

    // Launch
    outputChannel.appendLine('── Launching ────────────────────────────────');
    const r = cfg;
    const launchArgs = [
        '--wheel-radius',    String(r.get<number>('robot.wheelRadiusIn',         1.625)),
        '--track-width',     String(r.get<number>('robot.trackWidthIn',           12.0)),
        '--max-rpm',         String(r.get<number>('robot.maxRpm',                 600)),
        '--gear-ratio',      String(parseGearRatio(r.get<unknown>('robot.gearRatio', '1:1'))),
        '--mass',            String(r.get<number>('robot.massKg',                 10.0)),
        '--drive-motors',    String(r.get<number>('robot.driveMotorCount',        6)),
        '--gear-friction',   String(r.get<number>('robot.gearFriction',           0.15)),
        '--left-ports',      (r.get<number[]>('robot.leftPorts',  [4,5,6])).join(','),
        '--right-ports',     (r.get<number[]>('robot.rightPorts', [1,2,3])).join(','),
        '--tracking-left',   String(r.get<number>('robot.trackingLeftPort',       0)),
        '--tracking-right',  String(r.get<number>('robot.trackingRightPort',      0)),
        '--tracking-mid',    String(r.get<number>('robot.trackingMidPort',        0)),
        '--tracking-radius', String(r.get<number>('robot.trackingWheelRadiusIn',  1.0)),
        '--robot-width',     String(r.get<number>('robot.widthIn',                8.0)),
        '--robot-height',    String(r.get<number>('robot.heightIn',               8.0)),
        '--start-x',         String(r.get<number>('robot.startX',                 0.0)),
        '--start-y',         String(r.get<number>('robot.startY',                 0.0)),
        '--start-heading',   String(r.get<number>('robot.startHeadingDeg',        0.0)),
    ];

    outputChannel.appendLine(exePath + ' ' + launchArgs.join(' '));

    const simCwd = path.join(bd, 'sim');
    const child  = cp.spawn(exePath, launchArgs, { cwd: simCwd, detached: true, stdio: 'ignore' });
    child.unref();
}

function killRunningSimAsync(): Promise<void> {
    return new Promise(resolve => {
        const proc = cp.spawn('taskkill', ['/F', '/IM', 'vex_sim.exe'], { shell: true });
        proc.on('close', () => resolve());
    });
}

function runCommand(cmd: string, args: string[], cwd: string): Promise<boolean> {
    return new Promise(resolve => {
        outputChannel.appendLine(`> ${path.basename(cmd)} ${args.join(' ')}`);
        let done = false;
        const finish = (ok: boolean) => { if (!done) { done = true; resolve(ok); } };

        // shell:true routes through cmd.exe which handles the quoted path correctly.
        // We quote cmd manually so cmd.exe sees: "C:\Program Files\cmake.exe" args...
        // Node.js sets windowsVerbatimArguments automatically for shell:true, so
        // the inner quotes survive the cmd.exe /s /c wrapper intact.
        const proc = cp.spawn(`"${cmd}"`, args, {
            cwd, shell: true, env: buildEnv(),
        });
        proc.stdin?.end();  // send EOF so cmake never blocks on stdin
        proc.on('error', (err) => {
            outputChannel.appendLine(`\nFailed to start process: ${err.message}`);
            vscode.window.showErrorMessage(
                `VEX Sim: could not start ${path.basename(cmd)}: ${err.message}`
            );
            finish(false);
        });
        proc.stdout?.on('data', (d: Buffer) => outputChannel.append(d.toString()));
        proc.stderr?.on('data', (d: Buffer) => outputChannel.append(d.toString()));
        proc.on('close', code => {
            if (code !== 0) {
                outputChannel.appendLine(`\nError: command exited with code ${code}`);
                vscode.window.showErrorMessage(
                    `VEX Sim: ${path.basename(cmd)} failed (exit ${code}). See Output > VEX Sim for details.`
                );
                finish(false);
            } else {
                finish(true);
            }
        });
    });
}

export function deactivate() {}
