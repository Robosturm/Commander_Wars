# oxygine-flow
oxygine-flow is an expansion for oxygine which provides a neat system for dialogue/scene organization based on an asynchronous model of events.

Documentation: https://github.com/oxygine/oxygine-framework/wiki/oxygine-flow

Fully completed example is located in oxygine-flow\examples\HelloFlow

Video demonstration of this example: https://www.youtube.com/watch?v=Z4p1ol1-cLM&feature=youtu.be

Web version: http://oxygine.org/online_demo.php  (find Oxygine-Flow example)

##Example
see https://github.com/oxygine/oxygine-flow/blob/master/examples/HelloFlow/src/example.cpp

```cpp
flow::show(new MyScene, [](Event * event){
        log::messageln("scene closed");

        //show dialog
        flow::show(new MyDialog, [ = ](Event*){
                log::messageln("dialog closed");
            });
    });
```	
