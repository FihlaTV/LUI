
from panda3d.lui import LUIObject, LUISprite
from LUILabel import LUILabel
from LUIInitialState import LUIInitialState

class LUICheckbox(LUIObject):

    """ This is a simple checkbox, including a Label. The checkbox can either
    be checked or unchecked. """

    def __init__(self, checked=False, label=u"Checkbox", **kwargs):
        """ Constructs a new checkbox with the given label and state. """
        LUIObject.__init__(self, x=0, y=0, w=0, h=0, solid=True)
        LUIInitialState.init(self, kwargs)
        self._checkbox_sprite = LUISprite(self, "Checkbox_Default", "skin")
        self._label = LUILabel(parent=self, text=label, shadow=True, left=self._checkbox_sprite.width + 6)
        self._label.top = self._label.height - self._checkbox_sprite.height
        self._label.bind("resized", self._on_label_resized)
        self.fit_to_children()

        self._checked = checked
        self._update_sprite()

    def _on_label_resized(self, event):
        """ Internal handler when the text of the label got changed """
        self.fit_to_children()

    def on_click(self, event):
        """ Internal onclick handler. Do not override """
        self._checked = not self._checked
        self.trigger_event("changed")
        self._update_sprite()

    def on_mousedown(self, event):
        """ Internal mousedown handler. Do not override """
        self._checkbox_sprite.color = (0.86,0.86,0.86,1.0)

    def on_mouseup(self, event):
        """ Internal on_mouseup handler. Do not override """
        self._checkbox_sprite.color = (1,1,1,1)

    def toggle_checked(self):
        """ Toggles the checkbox state """
        self.set_checked(not self.get_checked())

    def set_checked(self, checked):
        """ Sets the checkbox state """
        self._checked = checked
        self._update_sprite()

    def get_checked(self):
        """ Returns a boolean wheter the checkbox is currently checked """
        return self._checked

    checked = property(get_checked, set_checked)

    def get_label(self):
        """ Returns a handle to the label, so it can be modified (e.g. change
            its text) """
        return self._label

    def _update_sprite(self):
        """ Internal method to update the sprites """
        img = "Checkbox_Checked" if self._checked else "Checkbox_Default"
        self._checkbox_sprite.set_texture(img, "skin")
