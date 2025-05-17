with open("../pyproject.toml") as reader:
    for line in reader:
        if line.startswith("version"):
            release = line.split('"')[1]
            break
html_title = project = f"pysorteddict {release}"
copyright = "2025, Vishal Pankaj Chandratreya"
author = "Vishal Pankaj Chandratreya"

extensions = ["myst_parser"]
source_suffix = [".md", ".rst"]

templates_path = ["_templates"]
exclude_patterns = ["_build"]

html_theme = "alabaster"
html_theme_options = {"fixed_sidebar": True, "page_width": "70%"}
html_use_index = False
