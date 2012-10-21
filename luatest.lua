local con = require("libconcol")

con.init()
-- init a resolution and font in case sdl backend is used
con.init_graphics(800, 600)

-- foreground white, bg red
con.setcolor(1, con.rgb(255,255,255))
con.setcolor(0, con.rgb(255,0,0))

con.goto(10, 20)
con.putchar("x", 1)

con.getkey()

con.cleanup()
