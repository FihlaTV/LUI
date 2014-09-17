

from panda3d.lui import *


class LUIButton(LUIObject):

    def __init__(self, text="Hello", width=100):
        LUIObject.__init__(self, x=0, y=0, w=width, h=50)

        self.sprite_left = self.attach_sprite("btn_left", "default")
        self.sprite_mid = self.attach_sprite("btn_mid", "default")
        self.sprite_right = self.attach_sprite("btn_right", "default")

        self.sprite_mid.set_width(
            width - self.sprite_left.get_width() - self.sprite_right.get_width())
        self.sprite_mid.set_left(self.sprite_left.get_width())

        self.sprite_right.set_left(
            self.sprite_mid.get_left() + self.sprite_mid.get_width())

        self.set_height(self.sprite_mid.get_height())

        self.text = LUIText(self, text, "default", 16.0)
        self.text.set_centered()
        self.text.set_relative_z_index(100)

        self.bind("mouseover", self.handle_event)
        self.bind("mouseout", self.handle_event)
        self.bind("click", self.on_click)

    def on_click(self, event):
        print "on click!"

    def handle_event(self, event):

        if event.get_name() == "mouseover":
            for child in [self.sprite_left, self.sprite_mid, self.sprite_right]:
                child.set_alpha(0.9)
        elif event.get_name() == "mouseout":
            for child in [self.sprite_left, self.sprite_mid, self.sprite_right]:
                child.set_alpha(1.0)


if __name__ == "__main__":

    # Test script for LUIButton
    from panda3d.core import *

    load_prc_file_data("", """

        text-minfilter linear
        text-magfilter linear

    """)
    import direct.directbase.DirectStart

    LUIAtlasPool.get_global_ptr().load_atlas(
        "default", "../Res/atlas.txt", "../Res/atlas.png")

    base.win.set_clear_color(Vec4(0.5,0.5,0.5,1))

    region = LUIRegion.make("LUI", base.win)
    handler = LUIInputHandler()
    base.mouseWatcher.attach_new_node(handler)
    region.set_input_handler(handler)

    button = LUIButton("Click Me")
    button.set_centered()
    region.root().add_child(button)


    base.accept("f3", region.root().ls)

    run()
