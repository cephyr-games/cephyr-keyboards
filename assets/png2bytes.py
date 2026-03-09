#!/usr/bin/env python3

from PIL import Image
import argparse


def int2hex(v):
    h = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"]
    d1 = v % 16
    d2 = v // 16
    return "0x" + h[d2] + h[d1]


def encode_vertical(path, threshold, indent, columns):
    img = Image.open(path).convert("L")
    width, height = img.size
    pixels = img.load()

    # extract hex encoded data
    data = []
    current = 0
    current_bit = 1
    x = 0
    y = 0

    if width % 8 != 0 or height % 8 != 0:
        print("Width and height must be multiples of 8 in vertical mode")
        return
    for i in range(width * height):
        if pixels[x, y] >= threshold:
            current += current_bit
        current_bit *= 2
        if current_bit == 256:
            data.append(current)
            current_bit = 1
            current = 0
        y += 1
        if y % 8 == 0:
            y -= 8
            x += 1
        if x >= width:
            y += 8
            x = 0
    # print data
    line = ""
    for i, v in enumerate(data):
        line += int2hex(v) + ", "
        if (i + 1) % columns == 0:
            print((indent + 2) * " " + line)
            line = ""

    if line:
        print((indent + 2) * " " + line)


def encode_horizontal(path, threshold, indent, columns, double):
    img = Image.open(path).convert("L")
    width, height = img.size
    pixels = img.load()

    # extract hex encoded data
    data = []
    current = 0
    bit_pos = 0

    if double and (width % 4 != 0 or height % 4 != 0):
        print("Width and height must be multiples of 4 in double-horizontal mode")
        return
    if not double and (width % 8 != 0 or height % 8 != 0):
        print("Width and height must be multiples of 8 in horizontal mode")
        return
    for y in range(height):
        for x in range(width):
            if double:
                v = pixels[x, y]
                if v >= 192:
                    current += 3 << bit_pos
                elif v >= 128:
                    current += 2 << bit_pos
                elif v >= 64:
                    current += 1 << bit_pos
                bit_pos += 2
            else:
                if pixels[x, y] >= threshold:
                    current += 1 << bit_pos
                bit_pos += 1
            if bit_pos >= 8:
                data.append(current)
                current = 0
                bit_pos = 0
    # print data
    line = ""
    for i, v in enumerate(data):
        line += int2hex(v) + ", "
        if (i + 1) % columns == 0:
            print((indent + 2) * " " + line)
            line = ""

    if line:
        print((indent + 2) * " " + line)


def encode_img(path, threshold, indent, columns, vertical, double):
    if vertical:
        return encode_vertical(path, threshold, indent, columns)
    else:
        return encode_horizontal(path, threshold, indent, columns, double)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Convert pngs to monocolored c-style arrays."
    )
    parser.add_argument(
        "images", metavar="IMAGE", nargs="+", help="Path(s) to png images"
    )
    parser.add_argument(
        "-d",
        "--double",
        action="store_true",
        help="Use two bytes per pixel to represent a gradient of 4 values.\n"
        + "Threshold will be ignored, instead be [64, 128, 192]\n"
        + "double is not supported in this mode.",
    )
    parser.add_argument(
        "-t", "--threshold", type=int, default=128, help="Value threshold (0-255)"
    )
    parser.add_argument(
        "-n", "--filename", default="image", help="The name of the array"
    )
    parser.add_argument(
        "-v",
        "--vertical",
        action="store_true",
        help="Make a byte contain a columns of pixels each, left to right.\n"
        + "Default is rows, left to right.",
    )
    parser.add_argument(
        "-c",
        "--columns",
        type=int,
        default=4,
        help="Amount of bytes per line of output",
    )
    parser.add_argument(
        "-i",
        "--indent",
        type=int,
        default=0,
        help="Amount of indent spaces on everything",
    )
    args = parser.parse_args()
    img = Image.open(args.images[0]).convert("L")
    width, height = img.size
    image_count = len(args.images)
    if args.double:
        byte_count = width * height // 4
    else:
        byte_count = width * height // 8
    print(args.indent * " " + f"// Size {width}x{height}")
    if len(args.images) == 1:
        print(
            args.indent * " "
            + f"static const char PROGMEM {args.filename}[{byte_count}] = "
            + "{"
        )
        encode_img(
            args.images[0],
            args.threshold,
            args.indent,
            args.columns,
            args.vertical,
            args.double,
        )
    else:
        print(
            args.indent * " "
            + f"static const char PROGMEM {args.filename}[{image_count}][{byte_count}] = "
            + "{"
        )
        for i, image_path in enumerate(args.images):
            print((args.indent + 2) * " " + f"[{i}] = " + "{")
            encode_img(
                image_path,
                args.threshold,
                args.indent + 2,
                args.columns,
                args.vertical,
                args.double,
            )
            print((args.indent + 2) * " " + "},")
    print(args.indent * " " + "};")
