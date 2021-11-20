#pragma once

#ifndef ENTITY_HPP

#include "StateMachine.hpp"
#include "olcPGEX_InputMap.h"

class e_StateMachine : public StateMachine
{
public:
    e_StateMachine()
    {
        IdleState = AddState("idle");
        JumpState = AddState("jump");
        RunState = AddState("run");
        State = IdleState;
    }

    int IdleState;
    int JumpState;
    int RunState;

    friend class Entity;

private:
    void Logic(float delta) override;
    int GetTransition(float delta) override;
    void EnterState(int newState, int oldState) override {};
    void ExitState(int oldState, int newState) override {};
};

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
    }

    friend class e_StateMachine;

protected:
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
};

void e_StateMachine::Logic(float delta)
{
    Parent->ApplyGravityToVelocity(delta);

    if (State == IdleState)
    {
        std::cout << "idle\n";
    }
    if (State == RunState)
    {
        std::cout << "run\n";
    }
    if (State == JumpState)
    {
        std::cout << "jump\n";
    }
    
    Parent->Move(delta);
}

int e_StateMachine::GetTransition(float delta)
{
    if (State == IdleState && Parent->_ShouldRun())
        return RunState;
    if (State == RunState && !Parent->_ShouldRun())
        return IdleState;
    
    if (Parent->_ShouldJump())
        return JumpState;
    if (!Parent->_ShouldJump() && State == JumpState)
        return PreviousState;

    return State;
}

#endif