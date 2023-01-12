You will need to move to this directory `day2/session1` to execute the commands proposed.

### Exercise X. Testing TRestAnalysisPlot

In this exercise we will launch restManager to produce a combined plot using the TRestAnalysisPlot metadata class. This class allows to define plots through a configuration file so that we can produce the same systematic plots for different datasets.

#### First simple test

```
restManager --c axionPhysicsPlot.rml --f "../../data/run_*rayTracing*root"
```

#### Chaging the output level

To see the effect of output levels you may try to launch `restManager` with a higher verbose level adding the `--v` flag in the arguments.

You may use:

```
restManager --v 2 --c axionPhysicsPlot.rml --f "../../data/run_*rayTracing*root"
```

The `--v 2` flag is equal to `info` output level and it will override the verboseLevel given by the configuration file for each individual section or class definition.

#### Modifying the RML behaviour using system variables

At the `<globals>` section inside the RML file we have defined a variable named `FORMAT`. This variable it is used inside the configuration file but can be overwritten and modified externally using sytem enironment variables. This allows to control externally the behaviour of the RML (configuration file) and define templates with the most relevant parameters that need to be modified/controlled externally.

You may try to generate an output using the different formats supported by ROOT (e.g. pdf, C, root. jpeg). Try to generate a different image format, and 

```
export FORMAT=pdf
restManager --c axionPhysicsPlot.rml --f "../../data/run_*rayTracing*root"
```

Hint: Open the RML file to inspect it and see how the `FORMAT` variable is used inside the RML file.

#### Modifying the plots

We may now explore the different `<plot>` sections and decide to change one of the plots to draw something else.

In this exercise you may decide to use *any* of the variables that are listed inside the `data/README.md` description at the [rest-school repository](https:/github.com/rest-for-physics/rest-school/).

A possible proposal:
 - Replace the variables inside the plot named `FocalSpot` by the variables `offset_posX` and `offset_posY`.
 - Remove the variables inside the plot named `SolarDist` and replace by a single variable histogram `axionPhoton_fieldAverage`.

Additionally you may update the title and labels to match what we are drawing "Offset" instead of "Spot" or "Field average [T]". You may also play around with the histogram limits using the `range` field, or the binning number using the `nbins` field.


#### Adding new plots

You can also add a new plot, just copy/paste a complete `<plot>` entry and modify it as you did in the previous section. The only difference now is that you need to allocate an additional space at the `<canvas` define a grid that can host at least 5 plots, such as 3x2 or 2x3. The canvas size should be also updated according to the new number of plots.

