import os
import time
import zipfile
from pathlib import Path

import tomllib

with zipfile.ZipFile(assets_src := Path(__file__).with_name("assets.zip")) as zf:
    assets_dst, modification_time_tuples = assets_src.parent, {}
    for zi in zf.infolist():
        if not (target := assets_dst / zi.filename).exists():
            zf.extract(zi, assets_dst)
            modification_time_tuples[target] = zi.date_time
    # Set the modification and access times of the extracted files to what the
    # archive reports. This must be done in a separate loop because the
    # modification time of a directory gets updated when a file is extracted
    # into it.
    for target, modification_time_tuple in modification_time_tuples.items():
        modification_timestamp = time.mktime((*modification_time_tuple, 0, 0, -1))
        os.utime(target, (modification_timestamp, modification_timestamp))

# Show all available wheels similar to how PyPI does.
whl_dir = Path(__file__).with_name("extra") / "simple" / "pysorteddict"
with open(whl_dir / "index.html", "w") as writer:
    print("<!DOCTYPE html><html><head><title>Links for pysorteddict</title></head><body>", file=writer)
    for whl_file in sorted(whl_dir.iterdir(), reverse=True):
        if whl_file.suffix == ".whl":
            print(f'<a href="{whl_file.name}">{whl_file.name}</a><br />', file=writer)
    print("</body></html>", file=writer)

with open(Path(__file__).parents[1] / "pyproject.toml", "rb") as reader:
    metadata = tomllib.load(reader)["project"]
author = metadata["authors"][0]["name"]
copyright = f"2025–2026, {author}"
release = metadata["version"]
project = metadata["name"] + " " + release

extensions = ["jupyterlite_sphinx", "myst_parser", "sphinx_inline_tabs"]
source_suffix = [".md", ".rst"]
exclude_patterns = ["_build"]

html_css_files = ["styles/custom.css"]
html_extra_path = ["extra"]
html_favicon = "_static/images/favicon.svg"
html_logo = "_static/images/logo.svg"
html_static_path = ["_static"]
html_theme = "furo"
html_theme_activated_colour = "rgba(39, 180, 166, 0.2)"
html_theme_background_colour = "rgba(39, 180, 166, 0.1)"
html_theme_foreground_colour = "rgb(1, 140, 126)"
html_theme_options = {
    "light_css_variables": {
        "color-sidebar-item-background--current": html_theme_activated_colour,
        "color-sidebar-item-background--hover": html_theme_background_colour,
        "color-sidebar-link-text--top-level": html_theme_foreground_colour,
        "color-toc-item-text--active": html_theme_foreground_colour,
        "font-stack": "Signika pysorteddict, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--headings": "Signika pysorteddict, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--monospace": "JetBrainsMono pysorteddict, Consolas, Monaco, Liberation Mono, monospace",
        "tabs--label-border--active": html_theme_foreground_colour,
        "tabs--label-text--active": html_theme_foreground_colour,
        "tabs--label-border--active--hover": html_theme_foreground_colour,
        "tabs--label-text--active--hover": html_theme_foreground_colour,
    },
    "source_branch": "main",
    "source_directory": "docs",
    "source_repository": metadata["urls"]["Repository"],
}
html_title = project
html_use_index = False

myst_enable_extensions = ["attrs_inline", "colon_fence"]
myst_heading_anchors = 4
templates_path = ["_templates"]
