# AGENTS.md

C + GTK4 wizard/driver installer for Void Linux (`neko-store` binary, `meson.build`).

## Build / run
```bash
meson setup build && meson compile -C build && ./build/neko-store
# rebuild after config change: meson setup build --reconfigure
```
- Deps: `gtk4-devel pkg-config meson ninja` (see `template`, `.forgejo/workflows/build.yml`).
- No tests, lint, or typecheck — compile is the check.
- Run `./build/neko-store` from repo root; `get_resource_path()` checks `exe_dir` + `exe_dir/..` only.
- `main.c` forces `GSK_RENDERER=gl`; keep it.

## Where logic lives
- `src/main.c` → `src/window.c` (all UI), `src/apps_manager.c` (app list), `src/installer.c` (`install_app_async`), `src/app_card.c`, `src/mirror_manager.c`.
- Adding/changing an installable app = 3 places: function + `case` + `list_apps()` in `download/install.sh`, plus `AppInfo` entry in `apps_manager.c:apps[]` with matching `resources/<icon>.png`. App/mirror lists are plain arrays via `neko_apps_list()`/`neko_mirrors_list()` — no `GList` wrappers.
- `apps_manager.c` `INSTALL_APP(id)` just curls the remote `download/install.sh` and runs `bash /tmp/neko-install.sh <id>` — real commands live in `install.sh`, edit there. `[neko]`-prefixed stdout lines show in the progress label; non-zero exit = failure.
- `resources/*.png`, `data/style.css` are loaded at runtime by relative path, not bundled — keep paths stable.
- `template` is the xbps package recipe (`build_style=meson`); expects `build/neko-store` + `resources/` + `data/`, installs to `/usr/lib/Neko-Wizard` with symlink in `/usr/bin`. CI (`.forgejo/workflows/build.yml`) builds in `void-mklive` container only.

## Void-only assumptions
- System installs via `pkexec xbps-install -Sy` (native), `arxy` (Arch compat, gaming only), or `xbps` from z-repo-musl; services via `ln -s /etc/sv/<svc> /var/service/` (runit); AppImage installs go to `~/apps` without pkexec. No flatpak, no Nvidia (unsupported on musl). Don't "fix" these to sudo/apt/systemd.
