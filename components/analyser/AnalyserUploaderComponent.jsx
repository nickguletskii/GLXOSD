import React from "react";
import ReactGA from 'react-ga'
import classNames from "classnames";
import FileReaderProgressComponent from "components/analyser/FileReaderProgressComponent";

class AnalyserUploaderComponent extends React.Component {
	static propTypes() {
		return {onTextLoaded: React.PropTypes.func.isRequired};
	}

	constructor(props) {
		super(props);

		this.onSelect = this
			.onSelect
			.bind(this);
		this.onOpenClick = this
			.onOpenClick
			.bind(this);
		this.setLogText = this
			.setLogText
			.bind(this);

		this.state = {
			selectedFile: null,
			opening: false,
			error: null
		};
	}

	onSelect() {
		this.setState({selectedFile: this.refs.filePicker.files[0]});
	}

	onOpenClick(e) {
		e.preventDefault();
		this
			.fileReaderProgress
			.reader
			.readAsText(this.state.selectedFile);
		this.fileName = this.state.selectedFile.name;
		ReactGA.event({category: "Analyser", action: "Open clicked"});
	}

	get getFileTypeDetermined() {
		return "";
	}

	setLogText(text) {
		this
			.props
			.onTextLoaded(text, this.fileName);
	}

	get uploadGroup() {
		const getContent = () => {
			if (this.state.errorMessage) {
				return `Couldn't parse frame log: ${this.state.errorMessage}`;
			}
			return "Press 'Open' to load the selected file.";
		};
		return (
			<div>
				<fieldset
					className={classNames("form-group", {"has-danger": this.state.errorMessage})}>
					<label className="form-control-label" htmlFor="openButton">{getContent()}</label>
					<button
						id="openButton"
						className="btn btn-primary pull-xs-right"
						disabled={!this.state.selectedFile}
						onClick={this.onOpenClick}
						href="#">Open</button>
				</fieldset>
				<FileReaderProgressComponent
					ref={(x) => this.fileReaderProgress = x}
					onCompletion={this.setLogText}/>
			</div>
		);
	}

	render() {
		return (
			<div>
				<p>Please select a frame timing log. Supported frame timing log formats:
				</p>
				<ul>
					<li>GLXOSD v3 and later</li>
					<li>voglperf</li>
				</ul>
				<p>Old GLXOSD versions are not supported.</p>
				<form className="form" disabled={this.state.opening}>
					<fieldset className="form-group">
						<label className="custom-file w-100">
							<input type="file" id="filePicker" class="custom-file-input" ref="filePicker" onChange={this.onSelect}/>

							<span className="custom-file-control-glxosd">
								{this.state.selectedFile
									? this.state.selectedFile.name
									: "Please select a frame log..."}
							</span>
						</label>
					</fieldset>
					{this.uploadGroup}
				</form>
			</div>
		);
	}
}
export default AnalyserUploaderComponent;
