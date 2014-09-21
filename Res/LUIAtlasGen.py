
"""

LUI Atlas Generator

TODO: Command line interface

"""

from os import listdir
from os.path import join, isfile, isdir

from panda3d.core import TexturePool, loadPrcFileData, PNMImage

import sys
sys.path.insert(0, "../")

from panda3d.lui import LUIAtlasPacker

# Supress PNMImage warnings about incorrect sRGB profile
loadPrcFileData("", "notify-level-pnmimage error")
loadPrcFileData("", "notify-level-lui spam")


class AtlasEntry:

    def __init__(self, verbose_name, source_path):
        self.name = verbose_name
        self.path = source_path
        self.tex = TexturePool.load_texture(source_path)
        self.w = self.tex.get_x_size()
        self.h = self.tex.get_y_size()
        self.area = self.w * self.h
        self.assigned_pos = None

    def __repr__(self):
        return self.name



def generate_atlas(files, dest_dat, dest_png):
    entries = []

    virtual_atlas_size = 32
    all_entries_matched = False

    print "Loading", len(files), "entries .."
    for verbose_name, source in files:
        entries.append(AtlasEntry(verbose_name, source))

    entries = sorted(entries, key=lambda a: -a.area)

    while not all_entries_matched:
        print "Trying to pack into a", virtual_atlas_size, "x", virtual_atlas_size, "atlas .."

        packer = LUIAtlasPacker(virtual_atlas_size)
        all_entries_matched = True

        for entry in entries:
            uv = packer.find_position(entry.w, entry.h)

            if uv.get_x() < 0:
                print "  Not all images matched, trying next power of 2"
                all_entries_matched = False
                virtual_atlas_size *= 2
                break
            entry.assigned_pos = uv

    print "Matched entries, writing atlas .."

    atlas_description_content = ""
    dest = PNMImage(virtual_atlas_size, virtual_atlas_size, 4)

    for entry in entries:
        src = PNMImage(entry.w, entry.h, 4)
        entry.tex.store(src)

        if not src.has_alpha():
            src.add_alpha()
            src.alpha_fill(1.0)

        dest.copy_sub_image(
            src, int(entry.assigned_pos.get_x()), int(entry.assigned_pos.get_y()))

        atlas_description_content += "{0} {1} {2} {3} {4}\n".format(
            entry.name.replace(" ", "_"),
            int(entry.assigned_pos.get_x()),
            int(entry.assigned_pos.get_y()),
            entry.w, entry.h)

    dest.write(dest_png)

    with open(dest_dat, "w") as handle:
        handle.write(atlas_description_content)


if __name__ == "__main__":

    supported_extensions = ["png", "jpg", "gif", "dds", "tga"]
    root_dir = "."

    def recursively_collect_files(source_dir):
        files = listdir(source_dir)
        result = []
        for f in files:
            abs_path = join(source_dir, f)

            if isdir(abs_path):
                result += recursively_collect_files(abs_path)
            else:
                extension = f.split(".")[-1]
                verbose_name = ''.join(f.split(".")[:-1])
                if extension in supported_extensions and f != "atlas.png":
                    result.append(
                        (verbose_name, join(source_dir, f)))
        return result

    convert = recursively_collect_files(root_dir)

    generate_atlas(convert, "atlas.txt", "atlas.png")
