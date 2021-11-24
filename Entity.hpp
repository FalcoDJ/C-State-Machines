#pragma once

#ifndef ENTITY_HPP

#include "StateMachine.hpp"
#include "olcPGEX_InputMap.h"


class Entity;

// Derive a class from the abstract StateMachine base class
// (You can call it what ever you want).
class e_StateMachine : public StateMachine<Entity>
{
public:
    e_StateMachine()
    {
        // You add states to the states machine using the AddState() funciton
        // The function returns the id of the state as an int, so make sure you save it.
        // Here I am using a pair to save both the state name key and the id
        IdleState.first = AddState("idle"), IdleState.second = "idle";
        JumpState.first = AddState("jump"), JumpState.second = "jump";
        RunState.first = AddState("run"), RunState.second = "run";

        // Take this opportunity to set the default state
        State = IdleState.first;
    }

    std::pair<int, std::string> IdleState;
    std::pair<int, std::string> JumpState;
    std::pair<int, std::string> RunState;

private:
    // The Logic() function controls how you handle the states
    // It's definition is below.
    void Logic(float delta) override;

    // The Get transition function controls how to transition between states
    int GetTransition(float delta) override;

    // You can use the functions for whatever you need, I have not defined them here.
    // I would control my animations through these functions
    void EnterState(int newState, int oldState) override {};
    void ExitState(int oldState, int newState) override {};
};

// The class that will use the state machine has very little work to do.
// You use this class to define what will be done, but the statemachine defines
// how and when it will be done. The most important part is to call the StateMachine's
// Update() function when ever you update the object.
class Entity
{
public:
    Entity()
    {
        m_StateMachine.Intialize(this);

        m_JumpKey.Keys = {olc::Key::SPACE, olc::Key::UP};
        m_LeftKey.Keys = {olc::Key::A, olc::Key::LEFT};
        m_RightKey.Keys = {olc::Key::D, olc::Key::RIGHT};
    }

    void Update(float delta)
    {
        GetInput(delta);

        m_ShouldRun = m_InputX != 0;
        m_ShouldJump = m_Velocity.y < 0;
        m_IsJumping = m_IsJumping && m_ShouldJump;

        m_StateMachine.Update(delta);
    }

    void Draw(olc::PixelGameEngine *pge)
    {
        pge->DrawCircle(m_Position, 8);
        pge->DrawString(m_Position + olc::vf2d(-16.0f, -20.0f), m_CurrentState_Name);
    }

    friend class e_StateMachine;

protected:
    // The are the functions that the StateMachine uses to control the player
    // The player itself will not call these functions.

    void GetInput(float delta)
    {
        float x_input = (int)m_RightKey.IsHeld - (int)m_LeftKey.IsHeld;
        m_InputX = x_input;

        if (m_JumpKey.IsPressed && !m_IsJumping)
        {
            m_IsJumping = true;
            m_Velocity.y = m_JumpSpeed;
        }
    }

    void ApplyGravityToVelocity(float delta)
    {
        m_Velocity.y += 98 * delta;
    }

    void Move(float delta)
    {
        m_Velocity.x = m_Speed * m_InputX;
        m_Position += m_Velocity * delta;
    }

    bool _ShouldRun()
    {
        return m_ShouldRun;
    }
    bool _ShouldJump()
    {
        return m_ShouldJump;
    }

private:
    e_StateMachine m_StateMachine;
    olc::InputMap m_JumpKey;
    olc::InputMap m_LeftKey;
    olc::InputMap m_RightKey;

    float m_Speed = 180.0f;
    float m_JumpSpeed = -180.0f;
    bool m_IsJumping = false;
    float m_InputX = 0.0f;
    const float ACC_AND_FRC = 0.4f;
    olc::vf2d m_Velocity;
    olc::vf2d m_Position;

    bool m_ShouldRun = false;
    bool m_ShouldJump = false;

    std::string m_CurrentState_Name = "";
};

// In the StateMachine's Logic() function you define 
// the behavior of the player during each state.
void e_StateMachine::Logic(float delta)
{
    //Always apply gravity for every state
    Parent->ApplyGravityToVelocity(delta);

    // State specific logic goes inside of if conditions
    // if (State == someState) { do some stuff }
    if (State == IdleState.first)
    {
        Parent->m_CurrentState_Name = IdleState.second;

        // Call some idle related functions
    }
    if (State == RunState.first)
    {
        Parent->m_CurrentState_Name = RunState.second;
        
        // Call some run related functions
    }
    if (State == JumpState.first)
    {
        Parent->m_CurrentState_Name = JumpState.second;

        // Call some jump related functions
    }
    
    // Always move the character for every state
    Parent->Move(delta);
}

// In the StateMachine's GetTransition() function
// you can controll the transitions of states
// by returning an integer id for a state that
// you want to change to (this is why we save the id of each state)
int e_StateMachine::GetTransition(float delta)
{
    // You can control state specific transitions like so...

    if (State == IdleState.first && Parent->_ShouldRun())
        return RunState.first;
    if (State == RunState.first && !Parent->_ShouldRun())
        return IdleState.first;
    
    if (Parent->_ShouldJump())
        return JumpState.first;
    if (!Parent->_ShouldJump() && State == JumpState.first)
        return PreviousState;


    // If no transition is wanted return the current 
    // state and the StateMachine skip the transition.
    return State;
}

#endif