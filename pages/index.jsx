import React from "react";
import ImageGallery from "react-image-gallery";
import DocumentTitle from "react-document-title";

export default class HomeView extends React.Component {
	render() {
		const images = [
			{
				original: "/assets/images/screenshots/glxgears.png"
			}, {
				original: "/assets/images/screenshots/portal2_1.png"
			}
		];
		const rest = require("jade!fragments/index-rest.jade")();
		const gallery = process.env.STATIC_ENV
			? (
				<div>
					<img
						src="/assets/images/screenshots/glxgears.png"
						className="img-roudned center-block"/>
					<img
						src="/assets/images/screenshots/portal2_1.png"
						className="img-roudned center-block"/>
				</div>
			)
			: (<ImageGallery
				items={images}
				showThumbnails={false}
				showBullets
				autoPlay
				slideInterval={4000}/>);
		return (
			<DocumentTitle title="GLXOSD, an OSD and benchmarking tool for Linux">
				<div>
					<h1>GLXOSD, an OSD and benchmarking tool for Linux</h1>
					<div className="image-gallery-root">
						{gallery}
					</div>
					<div dangerouslySetInnerHTML={{
						__html: rest
					}}/>
				</div>
			</DocumentTitle>
		);
	}
}
