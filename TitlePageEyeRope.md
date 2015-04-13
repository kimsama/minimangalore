# 소개 #

줄로 장애물을 피해서 탈출

개발자 - 강수경

# 구현상의 문제점 #

다음의 문제점들이 있습니다:
  * 줄 구현
    * NeHe Rope Simulation - 로프 구현에 제일 적합할 듯.
      * http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=40
    * ODE 물리 엔진의 로프를 참조합니다.
    * 
The cable is divided into n segments. I use capped cylinders as geoms.
To connect them, I tried different joints **(ball&socket, universal & hinge2)**.
Because the cable needs to have a material attribute which describes the maximum angle it can be bend, I use hinge2 now. With the parameters dParamLoStop and dParamHiStop I can control how far the cable can be bend.
Using an AMotor dry friction in the joints is modelled (as described in the
documentation). To slow down the cable, while it's hanging down and swinging, the velocity of the bodies is taken each frame, a negative friction of it is added to the body (again, as described in the documentation :)

  * 
  * 

http://www.allegro.cc/forums/thread/549703
```
Rope::Rope(Flatland::World &world, vec2 start, vec2 end, float length, float tautness) : start(start), end(end), terrain(false)
{
    const float erp = 0.2;
    const float dx = 20;
    vec2 dir = (end - start).hat();
    vec2 delta = dir * dx;

    RopeSegment* previous = new RopeSegment(world, start, start + delta);
    segments.push_back(previous);
    start += delta;

    for (float x = dx; x < length; x += dx)
    {
        vec2 end = start + delta;
        RopeSegment* current = new RopeSegment(world, start, end);
        segments.push_back(current);
        world.Anchor(*(previous->GetObject()), *(current->GetObject()), start, tautness, erp);
        previous = current;
        start = end;
    }
    previous->GetObject()->SetCenter(end - delta / 2);
}
```

```
dJointID World::Anchor(Object& o1, Object& o2, const vec2& point, float mu, float erp)
{
    dJointID joint = dJointCreateHinge(world, 0);
    dJointAttach(joint, o1.GetBody(), o2.GetBody());
    dJointSetHingeAnchor(joint, point.x, point.y, 0);
    dJointSetHingeAxis(joint, 0, 0, 1);
    dJointSetErp(joint, erp);

    if (mu)
    {
        dJointID friction = dJointCreateAMotor(world, 0);
        dJointAttach(friction, o1.GetBody(), o2.GetBody());

        dJointSetAMotorNumAxes(friction, 1);
        dJointSetAMotorAxis(friction, 0, 1, 0, 0, 1);
        dJointSetAMotorParam(friction, dParamFMax, mu);
        dJointSetAMotorParam(friction, dParamVel, 0);
    }
    return joint;
}
```

  * 손과 줄을 붙혀야하는데 손 위치를 어떻게 찾을 것인가...
  * 3D 좌표를 2D로 옮겨오는 방법? D3D에는 있지만... Nebula엔 없는 건지 못찾는 건지;;;;