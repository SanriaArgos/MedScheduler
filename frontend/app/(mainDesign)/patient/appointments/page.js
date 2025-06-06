"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function PatientAppointmentsPage() {
    const [appointments, setAppointments] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [cancelingId, setCancelingId] = useState(null);
    const [cancelSuccess, setCancelSuccess] = useState("");
    const router = useRouter();
    const [userId, setUserId] = useState(null);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'patient') {
            router.push('/login');
            return;
        }

        setUserId(userData.userId);
        fetchAppointments(userData.userId);
    }, [router]);

    const fetchAppointments = async (patientId) => {
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_patient_appointments?patient_id=${patientId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                // Сортируем записи по дате и времени
                const sortedAppointments = data.appointments.sort((a, b) => {
                    const dateA = new Date(`${a.appointment_date}T${a.appointment_time}`);
                    const dateB = new Date(`${b.appointment_date}T${b.appointment_time}`);
                    return dateA - dateB;
                });

                setAppointments(sortedAppointments);
            } else {
                setError(data.error || "Ошибка при получении записей");
            }
        } catch (err) {
            console.error("Error fetching appointments:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    const handleCancelAppointment = async (appointmentId) => {
        setCancelingId(appointmentId);
        setError("");
        setCancelSuccess("");

        try {
            const response = await fetch('https://api.medscheduler.ru/cancel_appointment', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ appointment_id: appointmentId, patient_id: userId }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setCancelSuccess("Запись успешно отменена");

                // Обновляем список записей (удаляем отмененную)
                setAppointments(appointments.filter(app => app.appointment_id !== appointmentId));

                // Сбрасываем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setCancelSuccess("");
                }, 3000);
            } else {
                setError(data.error || "Ошибка при отмене записи");
            }
        } catch (err) {
            console.error("Error canceling appointment:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setCancelingId(null);
        }
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric'
        });
    };

    // Форматирование времени
    const formatTime = (timeStr) => {
        return timeStr.substring(0, 5); // Берем только HH:MM из HH:MM:SS
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка записей...</div>
            </div>
        );
    }

    return (
        <div className="max-w-4xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Мои записи к врачам</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {cancelSuccess && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {cancelSuccess}
                </div>
            )}

            {appointments.length === 0 ? (
                <div className="bg-white shadow-md rounded-lg p-6 text-center">
                    <p className="text-lg text-gray-600 mb-4">У вас нет активных записей к врачам</p>
                    <Link href="/search" className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors">
                        Найти врача
                    </Link>
                </div>
            ) : (
                <div className="bg-white shadow-md rounded-lg overflow-hidden">
                    <table className="min-w-full divide-y divide-gray-200">
                        <thead className="bg-gray-50">
                            <tr>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Врач
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Клиника
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Дата и время
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Кабинет
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Действия
                                </th>
                            </tr>
                        </thead>
                        <tbody className="bg-white divide-y divide-gray-200">
                            {appointments.map((appointment) => (
                                <tr key={appointment.appointment_id}>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <Link href={`/doctor/${appointment.doctor_id}`} className="text-main hover:text-main2">
                                            <div className="text-sm font-medium">{appointment.doctor_name}</div>
                                            <div className="text-sm text-gray-500">{appointment.specialty}</div>
                                        </Link>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-500">{appointment.hospital_name}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-900">{formatDate(appointment.appointment_date)}</div>
                                        <div className="text-sm text-gray-500">{formatTime(appointment.appointment_time)}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-900">{appointment.cabinet}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap text-sm font-medium">
                                        <button
                                            onClick={() => handleCancelAppointment(appointment.appointment_id)}
                                            disabled={cancelingId === appointment.appointment_id}
                                            className="text-red-600 hover:text-red-900 disabled:opacity-50"
                                        >
                                            {cancelingId === appointment.appointment_id ? "Отмена..." : "Отменить запись"}
                                        </button>
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            )}

            <div className="flex justify-between mt-8">
                <Link href="/dashboard" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться в личный кабинет
                </Link>
                <Link href="/patient/waitlist" className="text-main hover:text-main2 transition-colors">
                    Лист ожидания →
                </Link>
            </div>
        </div>
    );
}
