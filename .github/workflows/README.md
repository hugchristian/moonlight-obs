# GitHub Actions Workflows

This directory contains GitHub Actions workflows for continuous integration, testing, and release automation for the Moonlight OBS Plugin.

## Workflows

### CI Workflow (`ci.yml`)

**Trigger:** Push or pull request to `main`, `master`, or `develop` branches

**Purpose:** Build and test the plugin on multiple platforms

**Jobs:**
- **build-linux**: Builds the plugin on Ubuntu Linux
  - Installs OBS Studio and FFmpeg development libraries
  - Compiles the plugin using CMake
  - Uploads the compiled `.so` file as an artifact
  
- **build-macos**: Builds the plugin on macOS
  - Installs dependencies via Homebrew
  - Compiles the plugin (continues even if OBS libs are missing)
  - Uploads the compiled `.so` file as an artifact
  
- **build-windows**: Builds the plugin on Windows
  - Uses Visual Studio 2022
  - Compiles the plugin (continues even if dependencies are missing)
  - Uploads the compiled `.dll` file as an artifact

**Artifacts:** Platform-specific compiled plugin files are available for download from the Actions tab.

### Release Workflow (`release.yml`)

**Trigger:** Push of a version tag (e.g., `v1.0.0`, `v1.2.3`)

**Purpose:** Build release artifacts and create a GitHub release

**Jobs:**
- **build-and-release**: Matrix job that builds for Linux, macOS, and Windows
  - Compiles the plugin for each platform
  - Packages the plugin with data files
  - Creates platform-specific archives (`.tar.gz` for Linux/macOS, `.zip` for Windows)
  - Uploads packaged artifacts
  
- **create-release**: Creates a GitHub release
  - Downloads all platform artifacts
  - Creates a new release with auto-generated release notes
  - Attaches all platform packages to the release

**How to Create a Release:**

1. Ensure all changes are committed and pushed to the main branch
2. Create and push a version tag:
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```
3. The workflow will automatically build, package, and create the release
4. Check the "Releases" page on GitHub for the published release

### Lint Workflow (`lint.yml`)

**Trigger:** Push or pull request to `main`, `master`, or `develop` branches

**Purpose:** Check code quality and style

**Jobs:**
- **clang-format**: Checks C code formatting
  - Runs clang-format on all `.c` and `.h` files
  - Reports formatting issues (non-blocking)
  
- **cppcheck**: Runs static analysis
  - Checks for common C/C++ coding issues
  - Analyzes performance and portability concerns
  - Reports warnings (non-blocking)

## Artifacts

Build artifacts from the CI workflow are retained for 90 days by default and can be downloaded from:
1. Go to the "Actions" tab in the repository
2. Click on a workflow run
3. Scroll down to the "Artifacts" section
4. Download the desired platform artifact

## Permissions

The workflows require the following permissions:
- `contents: write` - For creating releases (release workflow only)
- Standard read permissions for code checkout

## Customization

### Adding Tests

To add actual tests to the CI workflow, modify the "Run tests" step in `ci.yml`:

```yaml
- name: Run tests
  run: |
    cd build
    ctest --output-on-failure
```

### Changing Build Configurations

To modify build settings, update the CMake configuration step:

```yaml
- name: Configure CMake
  run: |
    cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DYOUR_CUSTOM_OPTION=ON
```

### Platform-Specific Dependencies

If you need to add or modify dependencies, update the "Install dependencies" steps for each platform.

## Troubleshooting

### Workflow Fails on macOS/Windows

Some workflows use `continue-on-error: true` for platforms where OBS development libraries may not be easily available. This is intentional for the initial setup. To fix:

1. Add platform-specific dependency installation steps
2. Or provide custom OBS library paths in the CMake configuration

### Missing Dependencies

If builds fail due to missing dependencies:

1. Check the workflow logs in the Actions tab
2. Add the required packages to the "Install dependencies" step
3. Commit and push the updated workflow

### Release Not Created

Ensure you have pushed a tag that matches the pattern `v*`:

```bash
git tag -a v1.0.0 -m "Release 1.0.0"
git push origin v1.0.0
```

The tag must be pushed to the repository for the workflow to trigger.
