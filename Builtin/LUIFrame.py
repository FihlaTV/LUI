
from __future__ import print_function

from panda3d.lui import LUIObject
from LUILayouts import LUICornerLayout
from LUIInitialState import LUIInitialState
from LUIScrollableRegion import LUIScrollableRegion

__all__ = ["LUIFrame"]

class LUIFrame(LUIObject):

    # Frame styles
    FS_sunken = 1
    FS_raised = 2

    """ A container which can store multiple ui-elements """

    def __init__(self, inner_padding=5, scrollable=False, style=FS_raised, **kwargs):
        """ Creates a new frame with the given options and style. If scrollable
        is True, the contents of the frame will scroll if they don't fit into
        the frame height. inner_padding only has effect if scrollable is True.
        You can call fit_to_children() to make the frame fit automatically to
        it's contents."""
        LUIObject.__init__(self)

        # Each *style* has a different border size (size of the shadow). The
        # border size shouldn't get calculated to the actual framesize, so we
        # are determining it first and then substracting it.
        # TODO: We could do this automatically, determined by the sprite size
        # probably?
        self._border_size = 0
        self.padding = 10
        self.solid = True
        prefix = ""

        if style == LUIFrame.FS_raised:
            self._border_size = 33
            prefix = "Frame_"
        elif style == LUIFrame.FS_sunken:
            self._border_size = 5
            prefix = "SunkenFrame_"
        else:
            raise Exception("Unkown LUIFrame style: " + style)

        self._layout = LUICornerLayout(parent=self, image_prefix=prefix)

        self._effective_padding = self.padding_top + self._border_size
        self._scrollable = scrollable
        self._layout.margin = -self._effective_padding

        self._content = LUIObject(self)

        if self._scrollable:
            self._content.size = (self.width, self.height)
            self._content.pos = (self._border_size, self._border_size)
            self._scroll_content = LUIScrollableRegion(self._content,
                width=self.width-2*self.padding_left, height=self.height-2*self.padding_left,
                padding=inner_padding)
            self.content_node = self._scroll_content.content_node

        LUIInitialState.init(self, kwargs)

    def on_resized(self, event):
        """ Internal callback when the Frame got resized """
        self._layout.size = self.size + 2* self._border_size
        self._layout.update_layout()
