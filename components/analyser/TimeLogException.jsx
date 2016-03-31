export default class TimeLogException extends Error {
	constructor(message) {
		super(message);
		this.message = message;
	}
}
