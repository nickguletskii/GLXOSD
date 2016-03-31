import React from "react";
import DocumentTitle from "react-document-title";

import {prefixLink} from "gatsby-helpers";

export default class extends React.Component {
	static propTypes() {
		return {title: React.PropTypes.string, favicon: React.PropTypes.string, body: React.PropTypes.string};
	}
	render() {
		let title = DocumentTitle.rewind();
		if (this.props.title) {
			title = this.props.title;
		}

		let cssLink;
		if (process.env.NODE_ENV === "production") {
			cssLink = <link rel="stylesheet" href={prefixLink("/styles.css")}/>;
		}

		return (
			<html lang="en">
				<head>
					<meta charSet="utf-8"/>
					<meta httpEquiv="X-UA-Compatible" content="IE=edge"/>
					<meta
						name="viewport"
						content="width=device-width, initial-scale=1.0 maximum-scale=5.0"/>
					<meta
						name="Description"
						content="GLXOSD is an extensible on-screen display (OSD)/overlay for OpenGL applications running on Linux with X11 which aims to provide similar functionality to MSI Afterburner/RivaTuner OSD. It can show FPS, frame timings, temperatures and more in OpenGL games and applications. It can also be used to benchmark games, much like voglperf."/>
					<title>{title}</title>
					<link rel="shortcut icon" href={this.props.favicon}/> {cssLink}
				</head>
				<body>
					<div
						id="react-mount"
						dangerouslySetInnerHTML={{
						__html: this.props.body
					}}/>
					<script src={prefixLink("/bundle.js")}/>
				</body>
			</html>
		);
	}
}
