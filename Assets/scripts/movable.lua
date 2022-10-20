spd = 10.0

deltaVel = 0.0

if left_move() then
	deltaVel = deltaVel - spd
end
if right_move() then
	deltaVel = deltaVel + spd
end

set_vel_x(vel.x + deltaVel * delta_time())
