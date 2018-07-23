-- example terrain generator
local terrain_gen = {}

function terrain_gen.gen_func(x,y,z)
   if (y > 64) then
	  return 0
   else
	  return 1
   end
end


return terrain_gen
