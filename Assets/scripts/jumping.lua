planeEpsilon = 0.1
jump = 10.0

e = ecs:GetEntity(entity_id)
pos = e:GetPosition()
plane = e:GetBouncePlane()
v = e:GetVelocity()

if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
    if GetInputState(eIC_Jump) then
        v.y = jump
    end
end
