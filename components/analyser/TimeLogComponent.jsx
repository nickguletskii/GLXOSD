import React from "react";
import {toNumber as _toNumber} from "lodash";
import TimeLogProcessor from "components/analyser/TimeLogProcessor";
import GraphComponent from "components/analyser/GraphComponent";
import ZoomControl from "components/analyser/ZoomControl";

export default class TimeLogComponent extends React.Component {

	static propTypes() {
		return {text: React.PropTypes.string, fileName: React.PropTypes.string.isRequired};
	}

	constructor(props) {
		super(props);
		this.handleZoom = this
			.handleZoom
			.bind(this);
		this.updateRollPeriod = this
			.updateRollPeriod
			.bind(this);

		try {
			this.processor = null;
			this.processor = new TimeLogProcessor(props.text);
			this.state = this.initialState(props);
		} catch (err) {
			this.state = {
				error: err.message || err
			};
		}
	}

	componentWillReceiveProps(props) {
		try {
			this.processor = null;
			this.processor = new TimeLogProcessor(props.text);
			this.setState(this.initialState(props));
		} catch (err) {
			this.setState({
				error: err.message || err
			});
		}
	}

	initialState(props) {
		return {
			data: this.processor.graphs,
			zoom: {
				min: 0,
				max: this.processor.differences.length
			},
			rollPeriod: 1,
			error: null
		};
	}

	handleZoom(min, max) {
		this.setState({
			zoom: {
				min,
				max
			}
		});
	}

	updateRollPeriod(ev) {
		this.setState({
			rollPeriod: _toNumber(ev.target.value)
		});
	}

	render() {
		if (this.state.error) {
			return (
				<div className="card card-danger">
					<h3 className="card-header">{this.props.fileName}</h3>
					<div className="card-block card-inverse">
						<p className="card-text">Error: {this.state.error}</p>
					</div>
				</div>
			);
		}

		const diffGraph = (<GraphComponent
			dataName="differences"
			title="&Delta;t: Time between consecutive frames"
			xlabel="Frame"
			ylabel="Frequency (Hz)"
			data={this.state.data.differences}
			rollPeriod={this.state.rollPeriod}
			zoom={this.state.zoom}
			handleZoom={this.handleZoom}/>);
		const freqGraph = (<GraphComponent
			dataName="frequency"
			title="f: Frequency (&Delta;t<sup>-1</sup>)"
			xlabel="Frame"
			ylabel="Frequency (Hz)"
			data={this.state.data.frequencies}
			rollPeriod={this.state.rollPeriod}
			zoom={this.state.zoom}
			handleZoom={this.handleZoom}/>);
		const fpsGraph = (<GraphComponent
			dataName="fps"
			title="Number of frames in the previous second"
			xlabel="Frame"
			ylabel="Number of frames"
			data={this.state.data.fps}
			rollPeriod={1}
			zoom={this.state.zoom}
			handleZoom={this.handleZoom}/>);

		let frameLogType;
		if (this.processor.format.type === "GLXOSD") {
			frameLogType = `GLXOSD version ${this.format.version}`;
		} else {
			frameLogType = "voglperf or other";
		}
		const zoom = () => (<ZoomControl
			zoom={this.state.zoom}
			frameCount={this.processor.frames.length}
			setZoom={this.handleZoom}/>);

		const rollingAverage = () => (
			<div className="form-inline">
				<fieldset className="form-group">
					<label>
						Rolling average roll period
					</label>
					<input
						ref="rollPeriod"
						type="number"
						className="form-control"
						min={1}
						value={this.state.rollPeriod}
						onChange={this.updateRollPeriod}/>
				</fieldset>
			</div>
		);
		return (
			<div className="card">
				<h3 className="card-header">{this.props.fileName}</h3>
				<div className="card-block">Frame log type: {frameLogType}</div>
				<ul className="list-group list-group-flush">
					<li className="list-group-item">{zoom()}</li>
					<li className="list-group-item">{rollingAverage()}</li>
					<li className="list-group-item">{diffGraph}</li>
					<li className="list-group-item">{zoom()}</li>
					<li className="list-group-item">{rollingAverage()}</li>
					<li className="list-group-item">{freqGraph}</li>
					<li className="list-group-item">{zoom()}</li>
					<li className="list-group-item">{rollingAverage()}</li>
					<li className="list-group-item">{fpsGraph}</li>
					<li className="list-group-item">{zoom()}</li>
				</ul>
			</div>
		);
	}
}
