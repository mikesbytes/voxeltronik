conf = Config.new()
conf:load_from_file('res/config.conf')

n_game = Game.new()
n_game:set_config(conf)
n_game:init()

n_scene = Scene.new()

camera = Camera.new()
skybox = Skybox.new()

function init_scene()
   print('Initializing scene')
   skybox_task = RenderTask.new(skybox, camera, n_game:get_window())
end

function reinit_scene()

end

function update(delta)
   camera:update(delta)
end

function draw()
   skybox_task:draw()
end

n_scene:set_init_fn(init_scene)
n_scene:set_reinit_fn(reinit_scene)
n_scene:set_update_fn(update)
n_scene:set_draw_fn(draw)

n_game:set_scene(n_scene)
n_game:start()
