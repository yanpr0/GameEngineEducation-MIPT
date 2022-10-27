speed = 10.0
deltaVel = 0.0

if GetInputState(eIC_GoLeft) then
	deltaVel = deltaVel - speed
end
if GetInputState(eIC_GoRight) then
	deltaVel = deltaVel + speed
end

v = ecs:GetEntity(entity_id):GetVelocity()

v.x = v.x + deltaVel * delta_time()
