import re

with open("../pyproject.toml") as reader:
    release = re.search(r'^version = "(.*?)"$', reader.read(), flags=re.MULTILINE).group(1)
project = f"pysorteddict {release}"
copyright = "2025, Vishal Pankaj Chandratreya"
author = "Vishal Pankaj Chandratreya"

extensions = ["myst_parser"]
source_suffix = [".md", ".rst"]
exclude_patterns = ["_build"]

html_css_files = ["css/custom.css"]
html_favicon = "_static/images/favicon.svg"
html_logo = "_static/images/logo.svg"
html_static_path = ["_static"]
html_theme = "furo"
html_theme_options = {
    "light_css_variables": {
        "font-stack": "Livvic, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--headings": "Livvic, system-ui, sans-serif, Apple Color Emoji, Segoe UI Emoji",
        "font-stack--monospace": "Menlo, Consolas, Monaco, Liberation Mono, Lucida Console, monospace",
    },
    "source_branch": "main",
    "source_directory": "docs",
    "source_repository": "https://github.com/tfpf/pysorteddict",
}
html_title = project
html_use_index = False
myst_heading_anchors = 3
