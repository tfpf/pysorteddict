from docutils import nodes
from docutils.parsers.rst import Directive, directives

from sphinx.application import Sphinx
from sphinx.util.typing import ExtensionMetadata

class details(nodes.General, nodes.Element):
    pass

def visit_details(self, node: details):
    details_attributes = []
    if classes := node["class"]:
        details_attributes.append('class="' + " ".join(classes) + '"')
    if node["open"]:
        details_attributes.append("open")
    self.body.append("<details " + " ".join(details_attributes) + ">")
    self.body.append("<summary>" + node["summary"] + "</summary>")

def depart_details(self, node: details):
    self.body.append("</details>")

class DetailsDirective(Directive):
    final_argument_whitespace = True
    has_content = True
    option_spec = {"class": directives.class_option, "open": directives.flag}
    required_arguments = 1

    def run(self):
        node = details()
        node["class"] = self.options.get("class")
        node["open"] = "open" in self.options
        node["summary"] = self.arguments[0]
        self.state.nested_parse(self.content, self.content_offset, node)
        return [node]

def setup(app: Sphinx) -> ExtensionMetadata:
    app.add_node(details, html=(visit_details, depart_details))
    app.add_directive("details", DetailsDirective)
    return {
        'parallel_read_safe': True,
        'parallel_write_safe': True,
        'version': '0.0',
    }

