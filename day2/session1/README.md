
### Exercise X. Testing TRestAnalysisPlot

In this exercise we will launch restManager to produce a combined plot using the TRestAnalysisPlot metadata class. This class allows to define plots through a configuration file so that we can produce the same systematic plots for different datasets.

```
restManager --c axionPhysicsPlot.rml --f "../../data/run_*rayTracing*root"
```

To see the effect of output levels you may try to launch `restManager` with a higher verbose level adding the `--v` flag in the arguments.

You may use:

```
restManager --v 2 --c axionPhysicsPlot.rml --f "../../data/run_*rayTracing*root"
```

The `--v 2` flag is equal to `info` output level and it will override the verboseLevel given by the configuration file for each individual section or class definition.


