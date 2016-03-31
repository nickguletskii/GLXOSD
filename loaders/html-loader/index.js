import htmlFrontMatter from "html-frontmatter";
import objectAssign from "object-assign";
import jade from "jade";
import {
	startsWith as _startsWith
} from "lodash";
import frontMatter from "front-matter";

module.exports = function(content) {
	this.cacheable();

	let data;
	if(_startsWith(content, "---")) {
		const meta = frontMatter(content);
		data = objectAssign({}, meta.attributes, {
			body: jade.render(meta.body)
		});
	} else {
		const meta = htmlFrontMatter(content);
		data = objectAssign({}, meta, {
			body: content
		});
	}
	this.value = data;
	return `module.exports = ${JSON.stringify(data)}`;
};
