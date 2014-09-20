

from panda3d.lui import *


class LUIInputField(LUIObject):

    def __init__(self, width=200, font_size=20):
        LUIObject.__init__(self, x=0, y=0, w=width, h=50)

        self.background = LUISprite(
            self, "blank", "default", 0, 0, width, font_size + 10, (0.96, 0.96, 0.96, 1.0))
        self.background.z_offset = 5

        self.background_border = LUISprite(
            self, "blank", "default", -1, -1, width + 2, font_size + 12, (0.2, 0.6, 1.0, 1.0))

        self.text = LUIText(self, "Placeholder", "default", font_size)
        self.text.color = (0.2, 0.2, 0.2)
        self.text.margin.top = 3
        self.text.margin.left = 3
        self.text.z_offset = 10

        self.cursor = LUISprite(
            self, "blank", "default", x=0, y=0, w=2, h=font_size)
        self.cursor.color = (0.2, 0.2, 0.2)
        self.cursor.margin = (6, 0, 0, 8)
        self.cursor.z_offset = 20

        self._place_cursor()

    def on_click(self, event):
        print "Gain focus .."
        self.request_focus()

    def on_focus(self, event):
        print "Got focus .."

    def on_blur(self, event):
        print "Lost focus .."

    def _place_cursor(self):
        self.cursor.left = self.text.left + self.text.width

if __name__ == "__main__":

    # Test script for LUIInputField
    from panda3d.core import *

    load_prc_file_data("", """

        text-minfilter linear
        text-magfilter linear
        notify-level-lui debug
        text-pixels-per-unit 32
        sync-video #f

    """)
    import direct.directbase.DirectStart

    LUIFontPool.get_global_ptr().register_font(
        "default", loader.loadFont("../Res/font/SourceSansPro-Bold.ttf"))
    LUIAtlasPool.get_global_ptr().load_atlas(
        "default", "../Res/atlas.txt", "../Res/atlas.png")

    base.win.set_clear_color(Vec4(0.5, 0.5, 0.5, 1))

    region = LUIRegion.make("LUI", base.win)
    handler = LUIInputHandler()
    base.mouseWatcher.attach_new_node(handler)
    region.set_input_handler(handler)

    inputfield = LUIInputField(width=300, font_size=20)
    inputfield.parent = region.root()

    inputfield.centered = (True, True)

    base.accept("f3", region.root().ls)

    base.run()
