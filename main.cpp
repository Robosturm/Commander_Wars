#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"
#include "ox/DebugActor.hpp"

using namespace oxygine;

int main(int argc, char* argv[])
{
    /*************************************************************************************************/
    // setup stuff is done here
    ObjectBase::__startTracingLeaks();
    // Initialize Oxygine's internal stuff
    core::init_desc desc;
    desc.title = "Commander Wars";


    // The initial window size can be set up here on SDL builds, ignored on Mobile devices
    desc.w = 960;
    desc.h = 640;


    // Create the stage. Stage is a root node for all updateable and drawable objects
    Stage::instance = new Stage();
    Point size = core::getDisplaySize();
    getStage()->setSize(size);

    // DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
    //DebugActor::show();

    /*************************************************************************************************/
    // This is the main game loop.
    while (true)
    {
        // Update engine-internal components
        // If input events are available, they are passed to Stage::instance.handleEvent
        // If the function returns true, it means that the user requested the application to terminate
        bool done = core::update();

        // Update our stage
        // Update all actors. Actor::update will also be called for all its children
        getStage()->update();

        if (core::beginRendering())
        {
            Color clearColor(32, 32, 32, 255);
            Rect viewport(Point(0, 0), core::getDisplaySize());
            // Render all actors inside the stage. Actor::render will also be called for all its children
            getStage()->render(clearColor, viewport);

            core::swapDisplayBuffers();
        }
        // check for termination
        if (done)
        {
            break;
        }
    }
    /*************************************************************************************************/
    // clean up section ahead

    // If we get here, the user has requested the Application to terminate.
    // We dump and log all our created objects that have not been freed yet
    ObjectBase::dumpCreatedObjects();

    // Releases all internal components and the stage
    core::release();

    // The dump list should be empty by now,
    // we want to make sure that there aren't any memory leaks, so we call it again.
    ObjectBase::dumpCreatedObjects();

    ObjectBase::__stopTracingLeaks();
    //end

	return 0;
}
