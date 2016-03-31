import React from "react";
import classNames from "classnames";

class FileReaderProgressComponent extends React.Component {
	static propTypes() {
		return {onCompletion: React.PropTypes.func.isRequired, onFailure: React.PropTypes.func.isRequired};
	}
	constructor(props) {
		super(props);
		this.reader = new FileReader();
		this.reader.onabort = () => {
			this.update({status: "ready", progress: null});
		};
		this.reader.onloadstart = () => {
			this.update({status: "reading", progress: 0});
		};
		this.reader.onload = (e) => {
			this.update({status: "complete", progress: 100});
			this
				.props
				.onCompletion(e.target.result);
		};
		this.reader.onerror = () => {
			this.update({status: "error", progress: 100});
			this
				.props
				.onFailure();
		};
		this.reader.onprogress = (evt) => {
			this.setState({
				status: "reading",
				progress: 100 * evt.loaded / evt.total
			});
		};
		this.state = {
			status: "ready",
			progress: null
		};
	}
	update(diff) {
		this.setState(diff);
	}
	render() {
		const progressClass = classNames("progress", {
			"progress-success": this.state.status === "complete",
			"progress-danger": this.state.status === "error"
		});
		return this.state.progress
			? (
				<progress className={progressClass} value={this.state.progress} max="100">{this
						.state
						.progress
						.toFixed(1)}%</progress>
			)
			: false;
	}
}

export default FileReaderProgressComponent;
