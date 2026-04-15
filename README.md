# block-sync

block-sync is a tool designed to allow Prism launcher users to efficiently sync their instance states across different machines. This is particularly useful for those who primarily play on single-player worlds, but still desire the benefits of servers in having identical player progress across different machines.

Beyond it's practical application, this project is an ongoing endeavour in learning and applying basic C++ concepts and idioms that I learn while reading through the 3rd Edition of Bjarne Stroustrup's _**A Tour of C++**_. Hence, my development patterns are sometimes sporadic, ranging from days where I write lots of new code, or spend time refactoring old code over and over to reinforce gleaned principles.

## Checkpoints

_Ordering reflects a **small** amount of precedence: earlier rows tend to unblock or harden later work. It is still not a strict schedule - I'm trying to have fun and apply what I'm learning through reading._

| Checkpoint | Status |
| --- | --- |
| Chunked file hashing (SHA-256 per block) | Done |
| CLI: Prism path → `prismlauncher.cfg` → instance pick | Done |
| Robust cfg parsing & cross-platform paths | In Progress |
| Sweeping Error Handling | In Progress |
| Basic CI workflow | Planned |
| Prism/instance discovery in its own module | Planned |
| Manifest + sync logic | Needs Testing |
| Tests | Planned |
| Hashing (mmap) and other optimization | Planned |
| Migration from CLI to basic GUI | Planned |
| Monolith server integration | Planned |

## Directory layout

The current layout (`src/main.cpp`, `src/core/`, `src/util/`) is **provisional**. It is enough for early work and may move as the codebase grows.

When complexity warrants it, a likely direction is: a small **library target** for sync, hashing, and manifests (e.g. under `src/blocksync/` or similar), a thin **CLI entry** (e.g. `src/cli/main.cpp`), a **Prism-focused module** for launcher paths and config, optional **`include/<project>/`** headers if the library is reused or installed, and **`tests/`** laid out to mirror the library. None of that is required until maintenance or feature size makes the split worthwhile.

## Contributing

Currently I would like to continue being the sole contributor, due to the educational nature of the project. This section is mostly for completeness, since I don't expect this idea to take off and see massive interest anyway. With that said though, I **greatly** appreciate any feedback, comments, or new implementation ideas - I'm programming this off the dome so I certainly have made and will make poor design choices, so any pointers (lol) are very welcome.

*DISCLAIMER*: AI was used to assist in writing this markdown file.
