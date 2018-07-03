conf = config.new()
conf:load_from_file('res/config.conf')
log_info('X res: ' ..  conf:get_value('graphics.res.x', '800'))
