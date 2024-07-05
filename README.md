# TableAndChairs

Whenever I go too deep into inheritance, I sometimes tell myself that composition is better
than inheritance. For this project, I wanted to do both.

I implemented a table as a set of procedural cube components inherited from
UProceduralMeshComponent. I thought: what is a simple table if not 5 stretched cubes? And
what is a chair if not a mini table but with a back geometry stretched cube?
So I went that way. The main positive aspect of this approach is the modularity, the fact that
an object can have all the cube components it needs, without the need to create very long
arrays of vertices and triangles. The main negative aspect is the fact that, of course, all
these components call CreateMeshSection each time one must rendered, so instead of
having a very complex object that has only 1 section, you find yourself dealing with n
sections for n components. Luckily every cube is one section with 24 vertices so it’s not a
problem even in a medium/large scale environment.

My first step was to create a ProceduralCubeComponent that inherits from
ProceduralMeshComponent and I composed a LeggedSurfaceGeometry with these
components.

The table and the chairs inherit from the legged surface geometry actor, this enables the
user to have complete modularity with this architecture: if, for example, we want to create a
cube wardrobe or a bed, we can use legged surface geometry and inherit from it.
LeggedSurfaceGeometry handles already the fact that have 5 different procedural cube
components, it renders them and places them in the right place as if they were its legs
(based on the surface width and length).
The table also has a ChairPoolComponent that optimizes requests for chair spawning and
re-usage at runtime to avoid useless initializations.
For the selection process, I used ITCSelectableObjectInterface which permits to override its
functions to obtain the expected result, whatever it will be: it’s up to the user to define the
expected behaviors when one of the interface functions is called. In my project, this
interface goes with SelectableObjectComponent to retrieve the material parameters data
and manipulate those in real time for the selection/focus logic.

When the table is spawned and modified via widgets, a function CanRenderGeometry is
always called: This function checks if a table or a geometry object will collide with something
else during its construction or manipulation so the user can be notified via widgets.
For the UI I used a model view controller pattern with Unreal MVVMViewModel. All widgets
are made in code due to performance, only a few functionalities are implemented in
blueprints, like animations that are triggered when they are added to the screen.
The flow of execution starts with the TC game mode base that handles the deprojection of
the mouse screen position to a real-world location. Once it hits something it calls the
GeometrySpawner handles the request of a new object (in this case a table, although
we can select whatever derived LeggedSurfaceGeometry class the user wants) if the object
hit is the floor. Otherwise, the HUD will be called to handle the hit of a table, since we want to
show a widget on the screen. The HUD instantiates a MainPage that handles all the widget
logic, which works only through ViewModels requests since it’s safer than a direct call on the
objects themselves.
In general, most of the time I prefer to use cache-friendly containers like Arrays, but I also
inserted a TMap for faster research on keys thanks to the hashmap O(1) search time
complexity.

Thanks for the time you spent reading this brief README introduction file
