# CPP-State-Machines

This is an implementation of GameEndeavor's Finite State Machine in c++
It allows for easily controlled transitions between states, can be used
to help control when you want to play an animation, and makes debugging easier
because you break the code into chunks that you can diagnose.

## Usage

To use the statemachine you inherit from the base class, override the four
funtions that control behavior (`Logic`, `GetTransition`, `EnterState`, and `ExitState`), 
and then add the derived statemachine class as a member object to the object you want to
have a state machine (i.e. a character, or enemy).

### Example

You can find a detailed example of how to use this in [Entity.hpp](Entity.hpp)
