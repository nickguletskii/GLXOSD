import React from "react";
import {
	filter as _filter,
	inRange as _inRange,
	max as _max,
	min as _min,
	map as _map,
	mean as _mean,
	compact as _compact
} from "lodash";
import Dygraph from "react-dygraphs";

export default class GraphComponent extends React.Component {
	static propTypes() {
		const types = {
			data: React.PropTypes.array.isRequired,
			dataName: React.PropTypes.string.isRequired,
			zoom: React.PropTypes.object.isRequired,
			title: React.PropTypes.string.isRequired,
			xlabel: React.PropTypes.string.isRequired,
			ylabel: React.PropTypes.string.isRequired,
			handleZoom: React.PropTypes.func.isRequired
		};
		return types;
	}
	render() {
		const legendId = `graph-legend-${this.props.dataName}`;
		const selectedData = _filter(this.props.data, (x) => _inRange(x[0], this.props.zoom.min, this.props.zoom.max));
		return (
			<div className="card-block graph-container">
				<Dygraph
					className="graph-container"
					title={this.props.title}
					xlabel={this.props.xlabel}
					ylabel={this.props.ylabel}
					labels={[this.props.xlabel, this.props.ylabel]}
					labelsDiv={legendId}
					legend="always"
					includeZero
					labelsShowZeroValues
					labelsSeparateLines
					hideOverlayOnMouseOut={false}
					rollPeriod={this.props.rollPeriod}
					data={this.props.data}
					onZoom={this.props.handleZoom}
					dateWindow={[this.props.zoom.min, this.props.zoom.max]}
					valueRange={[null, null]}
					strokeWidth={2}/>
				<div className="graph-legend" id={legendId}></div>
				<table className="table table-hover">
					<tbody>
						<tr>
							<th scope="row">Minimum</th>
							<td>{(_min(_map(selectedData, x => x[1] || NaN)) || 0).toFixed(2)}</td>
						</tr>
						<tr>
							<th scope="row">Mean</th>
							<td>{(_mean(_compact(_map(selectedData, x => x[1]))) || 0).toFixed(2)}</td>
						</tr>
						<tr>
							<th scope="row">Maximum</th>
							<td>{(_max(_map(selectedData, x => x[1] || NaN)) || 0).toFixed(2)}</td>
						</tr>
					</tbody>
				</table>
			</div>
		);
	}
}
