# block-sync

block-sync is a tool designed to allow Prism launcher users to efficiently sync their instance states across different machines. This is particularly useful for those who primarily play on single-player worlds, but still desire the benefits of servers in having identical player progress across different machines.

Beyond it's practical application, this project is an ongoing endeavour in learning and applying basic C++ concepts and idioms that I learn while reading through the 3rd Edition of Bjarne Stroustrup's _**A Tour of C++**_. Hence, my development patterns are sometimes sporadic, ranging from days where I write lots of new code, or spend time refactoring old code over and over to reinforce gleaned principles.

## DISCLAIMER

I will most likely halt work on the project after I achieve the following functionalities/checkpoints:

* CI Implementation
* Syncing functionality
* Unit testing
* Packaging (if easy)
* Error handling
* MAYBE GUI (if not too time consuming)

I will not be following up with the server integration stuff since it falls pretty concretely outside of the scope of knowledge and implementing it would be pretty inefficient considering my current knowledge base.

## Checkpoints

_Ordering reflects a **small** amount of precedence: earlier rows tend to unblock or harden later work. It is still not a strict schedule - I'm trying to have fun and apply what I'm learning through reading._

| Checkpoint | Status |
| --- | --- |
| Chunked file hashing (SHA-256 per block) | Done |
| CLI: Prism path → `prismlauncher.cfg` → instance pick | Done |
| Robust cfg parsing & cross-platform paths | Done(ish) |
| Sweeping Error Handling | Deferred |
| Basic CI workflow | Deferred |
| Prism/instance discovery in its own module | Done |
| Manifest writing | Done |
| Sync (manifest parsing/processing) | Done |
| Tests | Planned |
| Hashing (mmap) and other optimization | Deferred |
| Migration from CLI to basic GUI | Deferred |
| Manifest Databas | 多分 Deferred |

## Directory layout

*To be revised.*

## Contributing

Message me if you want to contribute for some reason.

*DISCLAIMER*: AI was used to assist in writing this markdown file.
