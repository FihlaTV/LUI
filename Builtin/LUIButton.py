
from panda3d.lui import LUIObject
from LUILayouts import LUIHorizontalStretchedLayout
from LUILabel import LUILabel
from LUIInitialState import LUIInitialState

__all__ = ["LUIButton"]

class LUIButton(LUIObject):

    """ Simple button, containing three sprites and a label. """

    def __init__(self, text=u"Button", template="ButtonDefault", **kwargs):
        """ Constructs a new button. The template controls which sprites to use.
        If the template is "ButtonDefault" for example, the sprites
        "ButtonDefault_Left", "ButtonDefault" and "ButtonDefault_Right" will
        be used. The sprites used when the button is pressed should be named
        "ButtonDefaultFocus_Left" and so on then.

        If an explicit width is set on the button, the button will stick to that
        width, otherwise it will automatically resize to fit the label """
        LUIObject.__init__(self, x=0, y=0, w=0, h=0, solid=True)
        self._dynamic_width = "width" not in kwargs
        self._template = template
        self._label = LUILabel(parent=self, text=text, shadow=True, z_offset=1,
                                centered=(True, True), margin=(0, 0, 0, -1))
        if self._dynamic_width:
            width = int(self._label.width) + 20
        else:
            width = kwargs["width"]
        self._layout = LUIHorizontalStretchedLayout(parent=self, width=width, prefix=self._template)
        self.margin_left = -1
        self.fit_to_children()
        LUIInitialState.init(self, kwargs)

    def on_resized(self, event):
        """ Internal callback when the button gets resized """
        self._layout.width = self.width
        self._layout.recompute()

    def on_mousedown(self, event):
        """ Internal on_mousedown handler. Do not override """
        self._layout.prefix = self._template + "Focus"
        self._label.margin_top = 1

    def on_mouseup(self, event):
        """ Internal on_mouseup handler. Do not override """
        self._layout.prefix = self._template
        self._label.margin_top = 0

    def get_text(self, text):
        """ Returns the current label text of the button """
        return self._label.text

    def set_text(self, text):
        """ Sets the label text of the button """
        self._label.text = text
        if self._dynamic_width:
            self.width = int(self._label.width) + 20

    text = property(get_text, set_text)
