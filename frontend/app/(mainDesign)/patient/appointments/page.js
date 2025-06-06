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

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'patient') {
            router.push('/login');
            return;
        }

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

    const handleCancelAppointment = async (recordId) => {
        setCancelingId(recordId);
        setError("");
        setCancelSuccess("");

        try {
            const patientId = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}').userId;

            const response = await fetch('https://api.medscheduler.ru/cancel_appointment', {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ record_id: recordId, patient_id: patientId }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setCancelSuccess("Запись успешно отменена");

                // Обновляем список записей (удаляем отмененную)
                setAppointments(appointments.filter(app => app.record_id !== recordId));

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

    // Форматирование даты в удобочитаемый формат
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

    // Проверка, прошла ли уже дата приема
    const isPastAppointment = (dateStr, timeStr) => {
        const appointmentDate = new Date(`${dateStr}T${timeStr}`);
        return appointmentDate < new Date();
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
                        Найти врача и записаться
                    </Link>
                </div>
            ) : (
                <div>
                    <div className="bg-white shadow-md rounded-lg overflow-hidden">
                        <table className="min-w-full divide-y divide-gray-200">
                            <thead className="bg-gray-50">
                                <tr>
                                    <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                        Врач
                                    </th>
                                    <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                        Специальность
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
                                {appointments.map((appointment) => {
                                    const isPast = isPastAppointment(appointment.appointment_date, appointment.appointment_time);

                                    return (
                                        <tr key={appointment.record_id} className={isPast ? "bg-gray-50" : ""}>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm font-medium text-gray-900">
                                                    {appointment.doctor_last_name} {appointment.doctor_first_name} {appointment.doctor_patronymic}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{appointment.specialty}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{appointment.hospital_name}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-900">
                                                    {formatDate(appointment.appointment_date)}
                                                </div>
                                                <div className="text-sm text-gray-500">
                                                    {formatTime(appointment.appointment_time)}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
                                                {appointment.cabinet}
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap text-right text-sm font-medium">
                                                {!isPast && (
                                                    <button
                                                        onClick={() => handleCancelAppointment(appointment.record_id)}
                                                        disabled={cancelingId === appointment.record_id}
                                                        className="text-red-600 hover:text-red-900 disabled:opacity-50"
                                                    >
                                                        {cancelingId === appointment.record_id ? "Отмена..." : "Отменить"}
                                                    </button>
                                                )}
                                                {isPast && (
                                                    <span className="text-gray-400">Прошедшая запись</span>
                                                )}
                                            </td>
                                        </tr>
                                    );
                                })}
                            </tbody>
                        </table>
                    </div>
                </div>
            )}

            <div className="flex justify-between mt-8">
                <Link href="/patient/profile" className="text-main hover:text-main2 transition-colors">
                    ← Мой профиль
                </Link>
                <Link href="/patient/waitlist" className="text-main hover:text-main2 transition-colors">
                    Лист ожидания →
                </Link>
            </div>
        </div>
    );
}
