import React from "react";
import DocumentTitle from "react-document-title";
import {config} from "config";
import {TableOfContents} from "components/TableOfContents";

export default class extends React.Component {
	static propTypes() {
		return {route: React.PropTypes.object};
	}
	render() {
		const post = this.props.route.page.data;

		const toc = post.toc
			? (
				<div>
					<h2>Table of Contents</h2>
					<TableOfContents content={post.body}/>
				</div>
			)
			: null;
		return (
			<DocumentTitle title={`${post.title} | ${config.siteTitle}`}>
				<div className="markdown">
					<h1>{post.title}</h1>
					{toc}
					<div dangerouslySetInnerHTML={{
						__html: post.body
					}}/>
				</div>
			</DocumentTitle>
		);
	}
}
