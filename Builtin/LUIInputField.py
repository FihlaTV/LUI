
from panda3d.lui import LUIObject, LUISprite
from LUILabel import LUILabel
from LUIInitialState import LUIInitialState

class LUIInputField(LUIObject):

    """ Simple input field """

    def __init__(self, parent=None, width=200, placeholder=u"Enter some text ..", value=u"", **kwargs):
        LUIObject.__init__(self, x=0, y=0, w=width, h=0, solid=True)

        self._bg_left = LUISprite(self, "InputField_Left", "skin")
        self._bg_mid = LUISprite(self, "InputField", "skin")
        self._bg_right = LUISprite(self, "InputField_Right", "skin")

        self._text_content = LUIObject(self)
        self._text_content.margin = (5, 8, 5, 8)
        self._text_content.clip_bounds = (0,0,0,0)
        self._text_content.height = self._bg_mid.height - 10
        self._text_content.width = self.width - 16

        self._text_scroller = LUIObject(parent=self._text_content, x=0, y=0)
        self._text = LUILabel(parent=self._text_scroller, text=u"", shadow=True)

        self._cursor = LUISprite(
            self._text_scroller, "blank", "skin", x=0, y=0, w=2, h=15)
        self._cursor.color = (0.5, 0.5, 0.5)
        self._cursor.margin_top = 2
        self._cursor.z_offset = 20
        self._cursor_index = 0
        self._cursor.hide()

        self._value = value

        self._placeholder = LUILabel(parent=self._text_content, text=placeholder, shadow=False)
        self._placeholder.color = (1,1,1,0.5)

        self._bg_mid.width = self.width - self._bg_left.width - self._bg_right.width
        self._bg_mid.left = self._bg_left.width
        self._bg_right.left = self._bg_mid.width + self._bg_mid.left

        if len(self._value) > 0:
            self._placeholder.hide()

        self._tickrate = 1.0
        self._tickstart = 0.0

        self.fit_to_children()
        self._render_text()

        if parent is not None:
            self.parent = parent

        LUIInitialState.init(self, kwargs)

    def get_value(self):
        """ Returns the value of the input field """
        return self._value

    def set_value(self, value):
        """ Sets the value of the input field """
        self._value = value
        # QUESTION: Should we trigger a callback when the user changes the value hisself?
        self.trigger_event("changed", self._value)
        self._render_text()

    value = property(get_value, set_value)

    def _set_cursor_pos(self, pos):
        """ Internal method to set the cursor position """
        self._cursor_index = max(0, min(len(self._value), pos))
        self._reset_cursor_tick()

    def on_tick(self, event):
        """ Tick handler, gets executed every frame """
        frametime = globalClock.get_frame_time() - self._tickstart
        show_cursor = frametime % self._tickrate < 0.5 * self._tickrate
        if show_cursor:
            self._cursor.color = (0.5,0.5,0.5,1)
        else:
            self._cursor.color = (1,1,1,0)

    def _add_text(self, text):
        """ Internal method to append text """
        self._value = self._value[:self._cursor_index] + text + self._value[self._cursor_index:]
        self._set_cursor_pos(self._cursor_index + len(text))
        self._render_text()

    def on_click(self, event):
        """ Internal on click handler """
        self.request_focus()

    def on_mousedown(self, event):
        """ Internal mousedown handler """
        local_x_offset = self._text.text_handle.get_relative_pos(event.coordinates).x
        self._set_cursor_pos(self._text.text_handle.get_char_index(local_x_offset))
        self._render_text()

    def _reset_cursor_tick(self):
        """ Internal method to reset the cursor tick """
        self._tickstart = globalClock.getFrameTime()

    def on_focus(self, event):
        """ Internal focus handler """
        self._cursor.show()
        self._placeholder.hide()
        self._reset_cursor_tick()

        self._bg_left.color  = (0.9,0.9,0.9,1)
        self._bg_mid.color   = (0.9,0.9,0.9,1)
        self._bg_right.color = (0.9,0.9,0.9,1)

    def on_keydown(self, event):
        """ Internal keydown handler """
        key_name = event.message
        if key_name == "backspace":
            self._value = self._value[:max(0, self._cursor_index - 1)] + self._value[self._cursor_index:]
            self._set_cursor_pos(self._cursor_index - 1)
            self.trigger_event("changed", self._value)
            self._render_text()
        elif key_name == "delete":
            self._value = self._value[:self._cursor_index] + self._value[min(len(self._value), self._cursor_index + 1):]
            self._set_cursor_pos(self._cursor_index)
            self.trigger_event("changed", self._value)
            self._render_text()
        elif key_name == "arrow_left":
            self._set_cursor_pos(self._cursor_index - 1)
            self._render_text()
        elif key_name == "arrow_right":
            self._set_cursor_pos(self._cursor_index + 1)
            self._render_text()

    def on_keyrepeat(self, event):
        """ Internal keyrepeat handler """
        self.on_keydown(event)

    def on_textinput(self, event):
        """ Internal textinput handler """
        self._add_text(event.message)
        self.trigger_event("changed", self._value)

    def on_blur(self, event):
        """ Internal blur handler """
        self._cursor.hide()
        if len(self._value) < 1:
            self._placeholder.show()

        self._bg_left.color = (1,1,1,1)
        self._bg_mid.color = (1,1,1,1)
        self._bg_right.color = (1,1,1,1)

    def _render_text(self):
        """ Internal method to render the text """
        self._text.set_text(self._value)
        self._cursor.left = self._text.left + self._text.text_handle.get_char_pos(self._cursor_index) + 1
        max_left = self.width - 20

        if self._value:
            self._placeholder.hide()

        # Scroll if the cursor is outside of the clip bounds
        relX = self.get_relative_pos(self._cursor.get_abs_pos()).x
        if relX >= max_left:
            self._text_scroller.left = min(0, max_left - self._cursor.left)
        if relX <= 0:
            self._text_scroller.left = min(0, - self._cursor.left - relX)


