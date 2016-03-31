import React from "react";
import {toNumber as _toNumber} from "lodash";

export default class ZoomControl extends React.Component {
	static propTypes() {
		const types = {
			zoom: React.PropTypes.object.isRequired,
			setZoom: React.PropTypes.func.isRequired,
			frameCount: React.PropTypes.number.isRequired
		};
		return types;
	}

	constructor(props) {
		super(props);
		this.zoomOut = this
			.zoomOut
			.bind(this);
		this.updateZoom = this
			.updateZoom
			.bind(this);
	}

	shouldComponentUpdate(nextProps) {
		return nextProps.zoom.min !== this.props.zoom.min || nextProps.zoom.max !== this.props.zoom.max;
	}

	updateZoom(event, s) {
		let min = this.props.zoom.min;
		let max = this.props.zoom.max;
		if (s === "max") {
			max = _toNumber(event.target.value);
		} else {
			min = _toNumber(event.target.value);
		}
		if (min === this.props.zoom.min && max === this.props.zoom.max) {
			return;
		}
		this
			.props
			.setZoom(min, max);
	}

	zoomOut() {
		this
			.props
			.setZoom(0, this.props.frameCount);
	}

	render() {
		return (
			<div className="form-inline">
				<div className="form-group">
					<label>From frame:
					</label>
					<input
						type="number"
						className="form-control"
						value={this
						.props
						.zoom
						.min
						.toFixed(2)}
						onChange={(ev) => this.updateZoom(ev, "min")}
						min="0"
						max={this.props.zoom.max}/>
				</div>
				<div className="form-group">
					<label>To frame:
					</label>
					<input
						type="number"
						className="form-control"
						value={this
						.props
						.zoom
						.max
						.toFixed(2)}
						onChange={(ev) => this.updateZoom(ev, "max")}
						min={this.props.zoom.min}
						max={this.props.frameCount}/>
				</div>
				<div className="form-group pull-xs-right">
					<button
						type="button"
						className="btn btn-primary"
						onClick={this.zoomOut}
						href="#">Zoom out</button>
				</div>
			</div>
		);
	}
}
