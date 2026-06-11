from docutils import nodes
from docutils.parsers.rst import Directive, directives

from sphinx.application import Sphinx
from sphinx.util.typing import ExtensionMetadata

class details(nodes.General, nodes.Element):
    pass

def visit_details(self, node):
    details_attributes = []
    if cls := node.get("class"):
        details_attributes.append(f'class={cls}')
    if node.get("open"):
        details_attributes.append("open")
    self.body.append(f"<details {details_attributes}>")
    if summary := node.get("summary"):
        self.body.append(f"<summary>{summary}</summary>")

def depart_details(self, node):
    self.body.append("</details>")

class DetailsDirective(Directive):
    final_argument_whitespace = True
    has_content = True
    option_spec = {"class": directives.class_option, "open": directives.flag}

    def run(self):
        node = details()
        if cls := self.options.get("class"):
            node["class"] = cls
        if "open" in self.options:
            node["open"] = True
        if summary := self.options.get("summary"):
            node["summary"] = summary
        self.state.nested_parse(self.content, self.content_offset, node)
        return [node]

def setup(app: Sphinx) -> ExtensionMetadata:
    app.add_node(details, html=(visit_details, depart_details))
    app.add_directive("details", DetailsDirective)
