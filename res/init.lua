conf = Config.new()
conf:load_from_file('res/config.conf')

n_game = Game.new()
n_game:set_config(conf)
n_game:init()

n_scene = Scene.new()

camera = Camera.new()
skybox = Skybox.new()
handler = InputHandler.new()
tileset = Tileset.new()
world = World.new()

--local terrain_gen = require("res.terraingen")
noise = Noise.new(os.time()) 
bdec = BinaryDecorator.new(noise, 0, 1, 0)
world.terrain_gen:set_decorator(bdec)

vg = NVG.create()
NVG.create_font(vg, "mono", "res/fonts/DejaVuSansMono.ttf")
current_fps = 0

local looking_at = {
   block_type = 0,
   height = 0,
   light = 0
}
local place_voxel = false
local break_voxel = false
local voxel_type = 1

function look()
   -- mouse movement
   local x,y = sdl_get_relative_mouse_state()
   camera:rotate(x * sensitivity * 0.001, y * sensitivity * 0.001)
end

function init_scene()
   --testing random things
   log_info("Initializing Scene")

   sensitivity = conf:get_value("controls.mouse.sensitivity", 5.0)
   handler:set_event_fn(SDL_EventType.SDL_QUIT, function () n_game:stop() end)
   handler:set_event_fn(SDL_EventType.SDL_MOUSEMOTION, look)

   handler:set_action("Move Forward", "W")
   handler:set_action("Move Backward", "S")
   handler:set_action("Move Left", "A")
   handler:set_action("Move Right", "D")
   handler:set_action("Move Up", "Space")
   handler:set_action("Move Down", "Left Ctrl")

   handler:set_action("Type 1", "1")
   handler:set_action("Type 2", "2")
   handler:set_action("Type 3", "3")
   handler:set_action("Type 4", "4")
   handler:set_action("Type 5", "5")

   handler:set_action("Break Voxel", "Mouse Left")
   handler:set_action("Place Voxel", "Mouse Right")

   handler:set_action("Screenshot", "F1")

   sdl_relative_mouse_mode(1)
   camera:set_aspect_ratio(n_game:get_window():get_aspect_ratio())
   camera:set_position(vec3.new(0,70,0))
   skybox_task = RenderTask.new(skybox, camera, n_game:get_window())
   world_task = RenderTask.new(world, camera, n_game:get_window())

   --texture loading
   tileset:build_texture()
   tileset:update_texture_at(0, "res/stone.png")
   tileset:update_texture_at(1, "res/dirt.png")
   tileset:update_texture_at(2, "res/grass.png")
   tileset:update_texture_at(3, "res/test.png")
   tileset:update_texture_at(4, "res/test-blue.png")

   world.voxel_info:new_type({
		 tag = "vtk:stone",
		 name = "Stone",
		 transparent = false,
		 emission = 0,
		 textures = {
			all = 0
		 }
   })

   world.voxel_info:new_type({
		 tag = "vtk:dirt",
		 name = "Dirt",
		 transparent = false,
		 emission = 0,
		 textures = {
			all = 1
		 }
   })

   world.voxel_info:new_type({
		 tag = "vtk:sod",
		 name = "Sod",
		 transparent = false,
		 emission = 0,
		 textures = {
			all = 1,
			top = 2
		 }
   })

   world.voxel_info:new_type({
		 tag = "vtk:pinklight",
		 name = "Pink Light",
		 transparent = false,
		 emission = 0xF4B0,
		 textures = {
			all = 3
		 }
   })

   world.voxel_info:new_type({
		 tag = "vtk:bluelight",
		 name = "Blue Light",
		 transparent = false,
		 emission = 0x00F0,
		 textures = {
			all = 4
		 }
   })

   world:queue_chunk_loads_around_point(vec3.new(0,0,0), 8)
   log_info("Initialized Scene")
end

function reinit_scene()

end

function update(delta) -- called every frame
   handler:update() -- flush inputs
   camera:update(delta)
   world:update()

   current_fps = 1.0/delta

   if handler:is_action_down("Move Forward") then
	  camera:move_relative(vec3.new(0,0,1 * delta * 16));
   end
   if handler:is_action_down("Move Backward") then
	  camera:move_relative(vec3.new(0,0,-1 * delta * 16));
   end
   if handler:is_action_down("Move Left") then
	  camera:move_relative(vec3.new(-1 * delta * 16, 0, 0));
   end
   if handler:is_action_down("Move Right") then
	  camera:move_relative(vec3.new(1 * delta * 16,0,0));
   end
   if handler:is_action_down("Move Up") then
	  camera:move(vec3.new(0, 1 * delta * 16,0));
   end
   if handler:is_action_down("Move Down") then
	  camera:move(vec3.new(0, -1 * delta * 16,0));
   end

   if handler:is_action_down("Type 1") then
	  voxel_type = 1
   elseif handler:is_action_down("Type 2") then
	  voxel_type = 2
   elseif handler:is_action_down("Type 3") then
	  voxel_type = 3
   elseif handler:is_action_down("Type 4") then
	  voxel_type = 4
   elseif handler:is_action_down("Type 5") then
	  voxel_type = 5
   end

   raycast = RayCast.new(world)
   success, h_p, h_n = raycast:cast(camera:get_position(), camera:get_angle_vector(), 10)
   looking_at.block_type = world:get_voxel_type(ivec3.new(h_p))
   looking_at.height = world:get_height(ivec2.new(h_p.x, h_p.z))
   looking_at.light = world:get_light_level(ivec3.new(h_p + h_n))
   
   if handler:is_action_down("Break Voxel") and not break_voxel then
	  if success then
		 break_voxel = true
		 world:break_voxel(ivec3.new(h_p))
	  end
   elseif break_voxel and not handler:is_action_down("Break Voxel") then
	  break_voxel = false
   end
   
   if handler:is_action_down("Place Voxel") and not place_voxel then
	  if success then
		 place_voxel = true
		 world:place_voxel(ivec3.new(h_p + h_n), voxel_type)
	  end
   elseif place_voxel and not handler:is_action_down("Place Voxel") then
	  place_voxel = false
   end   


   -- screenshot
   if handler:is_action_down("Screenshot") then
	  n_game:get_window():save_screenshot("screenshots/capture_" .. os.time() .. ".bmp")
   end
   
end

function draw()
   skybox_task:draw()
   world_task:draw()

   NVG.begin_frame(vg, 1920, 1080, 1)
   NVG.font_size(vg, 14)
   NVG.font_face(vg, "mono")
   NVG.fill_color(vg, NVG.rgba(0,0,0,190))
   local debug_line_index = 19
   local function debug_line(text)
	  NVG.text(vg, 5, debug_line_index, text)
	  debug_line_index = debug_line_index + 17
   end
   debug_line("FPS: " .. current_fps)
   debug_line("Camera Pos: " .. camera:get_position():__tostring())
   debug_line("Block Type: " .. looking_at.block_type)
   debug_line("Max Height: " .. looking_at.height)
   debug_line("Light Level: " .. looking_at.light)
   debug_line("Selected Type: " .. world.voxel_info:get_type_by_id(voxel_type).name)

   NVG.begin_path(vg)
   NVG.rect(vg, (1920 / 2) - 25, (1080 / 2) - 3, 15, 6)
   NVG.fill_color(vg, NVG.rgba(153, 0, 204, 80))
   NVG.fill(vg);

   NVG.begin_path(vg)
   NVG.rect(vg, (1920 / 2) + 10, (1080 / 2) - 3, 15, 6)
   NVG.fill(vg)
   
   NVG.end_frame(vg);
   GL.front_face(GL.CW);
   GL.enable(GL.DEPTH_TEST)
   GL.enable(GL.CULL_FACE)
end

n_scene:set_init_fn(init_scene)
n_scene:set_reinit_fn(reinit_scene)
n_scene:set_update_fn(update)
n_scene:set_draw_fn(draw)

n_game:set_scene(n_scene)
n_game:start()
