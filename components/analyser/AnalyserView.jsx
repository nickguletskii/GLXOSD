import React from "react";
import ReactGA from 'react-ga'
import DocumentTitle from "react-document-title";
import {config} from "config";
import AnalyserUploaderComponent from "components/analyser/AnalyserUploaderComponent";
import TimeLogComponent from "components/analyser/TimeLogComponent";

class AnalyserView extends React.Component {
	constructor(props) {
		super(props);
		this.setText = this
			.setText
			.bind(this);

		this.state = {
			text: null,
			fileName: null
		};
	}

	setText(text, fileName) {
		this.setState({
			text: (text && text.length > 0)
				? text
				: "!!!EMPTY FILE!!!",
			fileName
		});
		ReactGA.event({category: "Analyser", action: "Log loaded"});
	}

	render() {
		const timeLog = (this.state.text
			? <TimeLogComponent text={this.state.text} fileName={this.state.fileName}/>
			: null);
		return (
			<DocumentTitle title={`Analyser | ${config.siteTitle}`}>
				<div>
					<h1>GLXOSD Analyser</h1>
					<AnalyserUploaderComponent onTextLoaded={this.setText}/>
					<div>{timeLog}</div>
				</div>
			</DocumentTitle>
		);
	}
}

export default AnalyserView;
