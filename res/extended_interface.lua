--vec3 operators
function vec3:__add(rhs)
   return vec3.new(self.x + rhs.x, self.y + rhs.y, self.z + rhs.z) 
end

function vec3:add(rhs)
   self.x = self.x + rhs.x
   self.y = self.y + rhs.y
   self.z = self.z + rhs.z
end

function vec3:__sub(rhs)
   return vec3.new(self.x - rhs.x, self.y - rhs.y, self.z - rhs.z)
end

function vec3:sub(rhs)
   self.x = self.x - rhs.x
   self.y = self.y - rhs.y
   self.z = self.z - rhs.z
end

function vec3:__mul(rhs)
   return vec3.new(self.x * rhs.x, self.y * rhs.y, self.z * rhs.z)
end

function vec3:mul(rhs)
   self.x = self.x * rhs.x
   self.y = self.y * rhs.y
   self.z = self.z * rhs.z
end
   
function vec3:__tostring()
   return self.x .. ", " .. self.y .. ", " .. self.z
end
