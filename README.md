A Time-Coherent Parallel Mesh Collision Detection Solver

I plan on using tinyobjloader to load a set of OBJ meshes, doing a broadphase AABB decomposition on the meshes, and then doing a parallel narrowphase detection with either OpenMP or CUDA backend, starting with OpenMP.
Eventually, I plan to integrate this into Chrono to use instead of Bullet for collision detection, if it is (as the goal is) faster.

Goals:
- Have a working, but awfully slow solver
- Make it tolerable
- Parallelize it with OpenMP
- Attempt OpenMP cache magic for narrowphase
- Beat Bullet
- (Long-term) add a CUDA backend
- (Harder) Make it time-coherent (use past broadphase to accelerate present broadphase)
