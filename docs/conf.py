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

html_logo = "logo.jpg"
html_theme = "furo"
html_theme_options = {
    "source_branch": "main",
    "source_directory": "docs",
    "source_repository": "https://github.com/tfpf/pysorteddict",
}
html_use_index = False
