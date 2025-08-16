import tomllib
import zipfile
from pathlib import Path
import os
import time

with zipfile.ZipFile(assets_src := Path("_static/assets.zip")) as zf:
    assets_dst = assets_src.parent
    for zi in zf.infolist():
        if (target := assets_dst / zi.filename).exists():
            continue
        zf.extract(zi, assets_dst)
        original_timestamp = time.mktime(zi.date_time + (0, 0, -1))
        os.utime(target, (original_timestamp, original_timestamp))
        print(f"{target} set to {original_timestamp}")
raise SystemExit(1)

with open("../pyproject.toml", "rb") as reader:
    metadata = tomllib.load(reader)["project"]
author = metadata["authors"][0]["name"]
copyright = f"2025, {author}"
release = metadata["version"]
project = metadata["name"] + " " + release

extensions = ["myst_parser"]
source_suffix = [".md", ".rst"]
exclude_patterns = ["_build"]

html_css_files = ["styles/custom.css"]
html_favicon = "_static/images/favicon.svg"
html_logo = "_static/images/logo.svg"
html_static_path = ["_static"]
html_theme = "furo"
html_theme_options = {
    "light_css_variables": {
        "font-stack": "Signika pysorteddict, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--headings": "Signika pysorteddict, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--monospace": "JetBrainsMono pysorteddict, Consolas, Monaco, Liberation Mono, monospace",
    },
    "source_branch": "main",
    "source_directory": "docs",
    "source_repository": metadata["urls"]["Repository"],
}
html_title = project
html_use_index = False

myst_heading_anchors = 4
templates_path = ["_templates"]
