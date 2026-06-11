from typing import ClassVar

from docutils import nodes
from docutils.parsers.rst import Directive, directives
from sphinx.application import Sphinx
from sphinx.util.typing import ExtensionMetadata


class DetailsNode(nodes.General, nodes.Element):
    pass


def visit_details(self, node: DetailsNode):
    details_attributes = []
    if classes := node["class"]:
        details_attributes.append('class="' + " ".join(classes) + '"')
    if node["open"]:
        details_attributes.append("open")
    self.body.append("<details " + " ".join(details_attributes) + ">")
    self.body.append("<summary>" + node["summary"] + "</summary>")


def depart_details(self, _node: DetailsNode):
    self.body.append("</details>")


class DetailsDirective(Directive):
    final_argument_whitespace: bool = True
    has_content: bool = True
    option_spec: ClassVar[dict] = {"class": directives.class_option, "open": directives.flag}
    required_arguments: int = 1

    def run(self):
        node = DetailsNode()
        node["class"] = self.options.get("class")
        node["open"] = "open" in self.options
        node["summary"] = self.arguments[0]
        self.state.nested_parse(self.content, self.content_offset, node)
        return [node]


def setup(app: Sphinx) -> ExtensionMetadata:
    app.add_node(DetailsNode, html=(visit_details, depart_details))
    app.add_directive("details", DetailsDirective)
    return {
        "parallel_read_safe": True,
        "parallel_write_safe": True,
        "version": "0.0",
    }
