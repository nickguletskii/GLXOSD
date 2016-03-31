import React from "react";
import htmlparser from "htmlparser2";
import {
	includes as _includes,
	indexOf as _indexOf,
	set as _set,
	concat as _concat,
	take as _take,
	flatMap as _flatMap,
	map as _map,
	compact as _compact
} from "lodash";

export class TableOfContents extends React.Component {
	static propTypes() {
		const propTypes = {
			children: React.PropTypes.object.isRequired,
			title: React.PropTypes.string.isRequired,
			name: React.PropTypes.string.isRequired,
			headingElement: React.PropTypes.string.isRequired
		};
		return propTypes;
	}
	render() {
		const cur = [0, 0, 0, 0];
		const index = (i, key) => {
			const most = _take(cur, i);
			const rest = cur[i];
			const mostWithChildren = _flatMap(most, (x) => [x, "children"]);
			return _concat(mostWithChildren, rest, key);
		};

		const tags = ["h2", "h3", "h4", "h5"];
		const arr = [];
		let i = null;
		const parser = new htmlparser.Parser({
			onopentag: (name, attribs) => {
				if (_includes(tags, name)) {
					i = _indexOf(tags, name);
					for (let j = i + 1; j < cur.length; j++) {
						cur[j] = 0;
					}
					cur[i]++;
					if (attribs.id) {
						_set(arr, index(i, "id"), attribs.id);
					}
				}
			},
			ontext: (text) => {
				if (i !== null) {
					_set(arr, index(i, "title"), text);
				}
			},
			onclosetag: (tagname) => {
				if (tagname === tags[i]) {
					i = null;
				}
			}
		}, {decodeEntities: true});
		parser.write(this.props.content);
		parser.end();
		const visitor = (x) => (
			<li key={`${x.id}-toc`}>
				<a href={`#${x.id}`}>{x.title}</a>
				{x.children
					? (
						<ul>
							{_map(_compact(x.children), visitor)}
						</ul>
					)
					: <span/>}
			</li>
		);
		return (
			<ul>
				{_map(_compact(arr), visitor)}
			</ul>
		);
	}
}
