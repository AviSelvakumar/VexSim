// Copies sim source into vscode-extension/sim-src/ before packaging.
// Run via: npm run prepare-sim

const fs   = require('fs');
const path = require('path');

const projectRoot = path.resolve(__dirname, '..', '..');
const dest        = path.resolve(__dirname, '..', 'sim-src');

console.log('Preparing sim source for packaging...');
console.log(`  Source: ${projectRoot}`);
console.log(`  Dest:   ${dest}`);

// Clean destination
fs.rmSync(dest, { recursive: true, force: true });
fs.mkdirSync(dest, { recursive: true });

// Filter out CMake build artifacts
function noBuildArtifacts(src) {
    const rel = path.relative(projectRoot, src);
    if (rel.includes('CMakeFiles'))   return false;
    if (src.endsWith('.obj'))         return false;
    if (src.endsWith('.obj.d'))       return false;
    if (src.endsWith('.a'))           return false;
    if (src.endsWith('.exe'))         return false;
    if (src.endsWith('.dll'))         return false;
    return true;
}

// Directories to bundle
const dirs = ['sim', 'stubs', 'robot', 'lemlib'];
for (const dir of dirs) {
    const src = path.join(projectRoot, dir);
    const dst = path.join(dest, dir);
    process.stdout.write(`  Copying ${dir}/... `);
    fs.cpSync(src, dst, { recursive: true, filter: noBuildArtifacts });
    console.log('done');
}

// Top-level files
for (const file of ['CMakeLists.txt', 'CMakePresets.json']) {
    fs.copyFileSync(path.join(projectRoot, file), path.join(dest, file));
    console.log(`  Copied  ${file}`);
}

// LemLib source — resolve junctions/symlinks before copying so vsce gets real files
const lemlibSrc = path.join(projectRoot, 'lemlib-src');
if (fs.existsSync(lemlibSrc)) {
    const realSrc = fs.realpathSync(lemlibSrc);  // follow junction to actual directory
    process.stdout.write(`  Copying lemlib-src/ (from ${realSrc})... `);
    fs.cpSync(realSrc, path.join(dest, 'lemlib-src'), { recursive: true });
    console.log('done');
} else {
    console.warn('  WARNING: lemlib-src/ not found — LemLib will not be bundled.');
}

console.log('\nSim source ready.');
