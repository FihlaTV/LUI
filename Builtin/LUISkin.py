
from panda3d.lui import LUIFontPool, LUIAtlasPool
from panda3d.core import Filename
import os
from os.path import join

class LUISkin:

    """ Abstract class, each skin derives from this class """

    # This is only for debugging, and stores the absolute path of the skin. Later
    # it should be fetched dynamically
    skin_location = ""

    def __init__(self):
        pass

    def load(self):
        """ Skins should override this. Each skin should at least provide the fonts
        'default' and 'label', and at least one atlas named 'skin' """
        raise NotImplementedError()

    def get_resource(self, pth):
        """ Turns a relative path into an absolute one, using the skin_location """
        return Filename.from_os_specific(join(self.skin_location, pth)).get_fullpath()


class LUIDefaultSkin(LUISkin):

    """ The default skin which comes with LUI """

    skin_location = os.path.dirname(os.path.abspath(__file__))

    def __init__(self):
        pass

    def load(self):
        LUIFontPool.get_global_ptr().register_font(
            "default", loader.loadFont(self.get_resource("font/SourceSansPro-Semibold.ttf")))

        labelFont = loader.loadFont(self.get_resource("font/SourceSansPro-Semibold.ttf"))
        labelFont.setPixelsPerUnit(32)

        LUIFontPool.get_global_ptr().register_font(
            "label", labelFont)

        headerFont = loader.loadFont(self.get_resource("font/SourceSansPro-Light.ttf"))
        headerFont.setPixelsPerUnit(80)

        LUIFontPool.get_global_ptr().register_font("header", headerFont)

        LUIAtlasPool.get_global_ptr().load_atlas("skin",
            join(self.skin_location, "res/atlas.txt"),
            self.get_resource("res/atlas.png"))
