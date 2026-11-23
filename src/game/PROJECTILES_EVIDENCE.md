# Player projectiles

projectiles.hpp/.cpp reconstruct constructors42d640/42fc60, updates42d730/42fd50, destructors42d7c0/42fde0 and entire terrain callbacks42e130/430a50. Resource loaders42d5e0/42fc00 establish ball.app/wbomb.app.

Payload offsets:00 pool token;04/08 x/y;0c/10 image width/height;14..20 rect(2,2,12,12);24 entity registry handle434b40 (callbacks42d810/42fe30);28 terrain registry431050 (callbacks42e130/430a50);2c/30 renderx/y;34/38 previousx/y;3c vx;40 vy(initial-10);44 collision count(initial0). Constructor preserves supplied pool token; owner state1 requires scheduler activation pass.

Update stores previous and render position, renders if present, increments vy up to11 then integrates x/y. Range gate abs(x-camera_x-320)<961 freezes updates outside it; it does NOT mark for deletion. Signed abs arithmetic's INT_MIN edge preserved. No y/coordinate clamp.

Terrain callback side1/2 uses431130 occupancy and up-to26-pixel correction plus swept-path scans. Ball bounces, increments counter, marks deletion on exactly3. Water bomb marks deletion on exactly1 and calls42b300 with kind42/callback42b3d0/destroy42b460/group8/x/y. Destructor unregisters434bb0 then4310c0, releases payload pool token then process token. Host supplies ownership and actual registry implementations.

ProjectileContext is required, with no default no-op implementation. Entity-target response42d810/42fe30 covers numerous enemies/items and is explicitly delegated via entity_collision(callbackID,projectile,targetKind,targetPointer); it is NOT yet claimed reconstructed here. Rendering supplied by host should draw ball.app/wbomb.app at2c/30, keyed0xfe, unmirrored. Terrain call431130 arguments retain descriptor offset14 as identity; host can use rect14..20. Explosion constructor42b300 must be provided. Unsupported dependencies must fail clearly.

Tests: MSVC x64 C++17 /W4 compile and run via verify_projectiles.cmd; exact registry callback IDs/order, initialization token preservation, pre-integration rendering, distance960 inclusive vs961 freeze, three-hit ball lifetime, first-hit bomb detonation, and unregister/free order passed. Terrain helper conversion is reproducible with tools/rebuild_projectile_terrain.py; source origin round3 files.
