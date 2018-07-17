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

   sdl_relative_mouse_mode(1)
   camera:set_aspect_ratio(n_game:get_window():get_aspect_ratio())
   skybox_task = RenderTask.new(skybox, camera, n_game:get_window())
   world_task = RenderTask.new(world, camera, n_game:get_window())

   --texture loading
   tileset:build_texture()
   tileset:update_texture_at(0, "res/stone.png")
   tileset:update_texture_at(1, "res/dirt.png")
   tileset:update_texture_at(2, "res/grass.png")
   tileset:update_texture_at(3, "res/test.png")
   tileset:update_texture_at(4, "res/test-blue.png")

   --voxel info
   world.voxel_info:set_transparent(0, true);
   world.voxel_info:set_transparent(1, false);
   world.voxel_info:set_transparent(2, false);
   world.voxel_info:set_transparent(3, false);
   world.voxel_info:set_transparent(4, false);

   world.voxel_info:set_all_texture_indexes(1, 0)
   world.voxel_info:set_all_texture_indexes(2, 1)
   world.voxel_info:set_texture_index(2, FaceDirection.TOP, 2)
   world.voxel_info:set_all_texture_indexes(3, 3)
   world.voxel_info:set_all_texture_indexes(4, 4)

   world:queue_chunk_loads_around_point(vec3.new(0,0,0), 16)
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
end

function draw()
   skybox_task:draw()
   world_task:draw()
end

n_scene:set_init_fn(init_scene)
n_scene:set_reinit_fn(reinit_scene)
n_scene:set_update_fn(update)
n_scene:set_draw_fn(draw)

n_game:set_scene(n_scene)
n_game:start()
