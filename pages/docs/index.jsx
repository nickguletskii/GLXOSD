import React from "react";
import {Link} from "react-router";
import config from "config";
import {prefixLink} from "gatsby-helpers";
import DocumentTitle from "react-document-title";

export default class Docs extends React.Component {
	render() {
		return (
			<DocumentTitle title={`Documentation | ${config.siteTitle}`}>
				<div>
					<Link to={prefixLink("/docs/use/")}>
						<h1>General usage instructions</h1>
					</Link>
					<Link to={prefixLink("/docs/config/")}>
						<h1>Configure</h1>
					</Link>
					<Link to={prefixLink("/docs/opts/")}>
						<h1>Command-line options</h1>
					</Link>
					<Link to={prefixLink("/docs/faq/")}>
						<h1>FAQ</h1>
					</Link>
				</div>
			</DocumentTitle>
		);
	}
}
