# block-sync

block-sync is a tool designed to allow Prism launcher users to efficiently sync their instance states across different machines. This is particularly useful for those who primarily play on single-player worlds, but still desire the benefits of servers in having identical player progress across different machines.

## Checkpoints

_Ordering reflects a **small** amount of precedence: earlier rows tend to unblock or harden later work. It is still not a strict schedule._

| Checkpoint | Status |
| --- | --- |
| Chunked file hashing (SHA-256 per block) | Done |
| CLI: Prism path → `prismlauncher.cfg` → instance pick | In progress |
| Robust cfg parsing & cross-platform paths | Planned |
| Basic CI workflow | Planned |
| Prism/instance discovery in its own module | Planned |
| Manifest + sync logic | Planned |
| Tests | Planned |
| Hashing optimization (mmap) | Planned |
| Migration from CLI to basic GUI | Planned |
| Monolith server integration | Planned |

## Directory layout

The current layout (`src/main.cpp`, `src/core/`, `src/util/`) is **provisional**. It is enough for early work and may move as the codebase grows.

When complexity warrants it, a likely direction is: a small **library target** for sync, hashing, and manifests (e.g. under `src/blocksync/` or similar), a thin **CLI entry** (e.g. `src/cli/main.cpp`), a **Prism-focused module** for launcher paths and config, optional **`include/<project>/`** headers if the library is reused or installed, and **`tests/`** laid out to mirror the library. None of that is required until maintenance or feature size makes the split worthwhile.
