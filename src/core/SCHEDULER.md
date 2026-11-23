# Process scheduler reconstruction

Evidence: recovered_functions_final/functions/0x44d290.c (10 sentinels), 0x44d300.c (append node with state 1), 0x44d370.c (unlink and destructor callback), 0x44d3a0.c (two-pass update/cleanup), 0x44d440.c (mark entire group for deletion), allocator 0x42a170.c (size-class pool with allocation token).

scheduler.hpp implements a portable semantic model. Nodes are appended, new state=1 nodes skip updates until cleanup changes them to 0; only group enable==1 updates, but all groups clean up. Traversal order is supplied explicitly. Destructor callbacks follow unlinking. Callback insertion is visible to traversal but newborn state suppresses its update. Reentrant tick is rejected.

Intentional safety difference: detached node storage is retained until scheduler destruction, because the original cleanup loop reads next after its destructor callback. Original pool reuse and dangling pointer behavior are not emulated. Immediate callback-driven deletion of arbitrary nodes and pool-slot reuse require scenario validation before claiming equivalence. Initial tables confirmed: enable=1 for ten groups; order=0,1,2,3,4,5,7,6,8,9. Data payload ownership remains with rebuilt gameplay modules. Null update/destroy callbacks are ignored; original would invoke them blindly. This is not a binary-layout-compatible x86 struct.

Build/test: verify_scheduler.cmd (MSVC x64 C++17 /W4); lifecycle, delayed activation, creation during update, order, exact enabled flag, disabled-group cleanup tested.
